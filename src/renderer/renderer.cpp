#include "renderer.hpp"

#include <utils/texture.hpp>

#include <renderer/input-manager.hpp>
#include <renderer/camera.hpp>
#include <renderer/debug.hpp>
#include <renderer/main-node.hpp>
#include <renderer/cube-map-node.hpp>
#include <renderer/post-process-node.hpp>
#include <renderer/deferred-lighting-node.hpp>
#include <renderer/gaussian-blur-node.hpp>
#include <renderer/instanced-node.hpp>
#include <renderer/shadow-mapping-node.hpp>
#include <renderer/light-wrapper.hpp>

#include <model/scene-graph.hpp>
#include <model/cube-map.hpp>
#include <model/components/material.hpp>
#include <model/components/transformation.hpp>
#include <model/components/point-light.hpp>
#include <model/components/direction-light.hpp>
#include <model/components/volume.hpp>
#include <model/components/instanced.hpp>
#include <model/type-id.hpp>
#include <model/component-manager.hpp>

#include <sstream>
#include <algorithm>

namespace leo
{

Renderer::Renderer(GLFWwindow *window,
                   InputManager *inputManager,
                   Camera *camera,
                   Shader shader,
                   SceneGraph &sceneGraph) : _shader(shader), _sceneGraph(sceneGraph),
                                             _gBufferShader("resources/shaders/basic.vs.glsl", "resources/shaders/gbuffer.frag.glsl"),
                                             _deferredLightingShader("resources/shaders/post-process.vs.glsl", "resources/shaders/deferred-lighting.frag.glsl"),
                                             _postProcessShader("resources/shaders/post-process.vs.glsl", "resources/shaders/reinhard-tone-mapping.frag.glsl"),
                                             _cubeMapShader("resources/shaders/cube-map.vs.glsl", "resources/shaders/cube-map.frag.glsl"),
                                             _instancingShader("resources/shaders/instancing.vs.glsl", "resources/shaders/instanced-basic.frag.glsl"),
                                             _gammaCorrectionShader("resources/shaders/post-process.vs.glsl", "resources/shaders/gamma-correction.frag.glsl"),
                                             _shadowMappingShader("resources/shaders/dir-shadow-mapping.vs.glsl", "resources/shaders/dir-shadow-mapping.frag.glsl"),
                                             _cubeShadowMapShader("resources/shaders/point-shadow-mapping.vs.glsl", "resources/shaders/point-shadow-mapping.frag.glsl", "resources/shaders/point-shadow-mapping.geo.glsl"),
                                             _sceneContext(this->_context),
                                             _extractCapedBrightnessShader("resources/shaders/post-process.vs.glsl", "resources/shaders/extract-caped-brightness.frag.glsl"),
                                             _hdrCorrectionShader("resources/shaders/post-process.vs.glsl", "resources/shaders/hdr-correction.frag.glsl"),
                                             _bloomEffectShader("resources/shaders/post-process.vs.glsl", "resources/shaders/bloom-effect.frag.glsl")

{
  this->_sceneGraph.watch(this);
  this->_setWindowContext(window, inputManager);
  this->_setCamera(camera);
  this->_init();
}

Renderer::~Renderer()
{
  if (_mainNode != nullptr)
    delete _mainNode;
}

void Renderer::_init()
{
  this->_context.init();
  this->_initFramebuffers();
  this->_visitSceneGraph();
}

void Renderer::_initFramebuffers()
{
  this->_main.addColorBuffer({true});
  this->_main.useRenderBuffer();

  ColorBufferOptions normalPositionBufferOptions;
  normalPositionBufferOptions.pixelFormat = GL_RGB;
  normalPositionBufferOptions.dataFormat = GL_RGB16F;
  normalPositionBufferOptions.dataType = GL_FLOAT;
  this->_gBuffer.addColorBuffer(normalPositionBufferOptions); // position buffer
  this->_gBuffer.addColorBuffer(normalPositionBufferOptions); // position buffer
  this->_gBuffer.addColorBuffer();                            // albedo buffer
  this->_gBuffer.addColorBuffer();                            // spec buffer
  this->_gBuffer.useRenderBuffer();                           // depth as usual

  this->_multisampled.addColorBuffer({true, 4});
  this->_multisampled.useRenderBuffer({4});

  this->_postProcess.addColorBuffer();
  this->_postProcess.useRenderBuffer();

  this->_extractCapedBrightnessFB.addColorBuffer({true});
  this->_extractCapedBrightnessFB.addColorBuffer();
  this->_extractCapedBrightnessFB.useRenderBuffer();

  this->_hdrCorrectionFB.addColorBuffer({true});
  this->_hdrCorrectionFB.useRenderBuffer();

  this->_blurFB.addColorBuffer();
  this->_blurFB.useRenderBuffer();

  this->_bloomEffectFB.addColorBuffer({true});
  this->_bloomEffectFB.useRenderBuffer();
}

void Renderer::_setWindowContext(GLFWwindow *window, InputManager *inputManager)
{
  this->_inputManager = inputManager;
  this->_window = window;
  this->_context.setWindowContext(*window, *inputManager);
}

void Renderer::_setCamera(Camera *camera)
{
  this->_camera = camera;
}

void Renderer::_visitSceneGraph()
{
  const Entity *root = this->_sceneGraph.getRoot();
  if (root)
    this->_visitSceneGraphRec(*root);
}

void Renderer::_visitSceneGraphRec(const Entity &root)
{
  for (auto &p : root.getComponents())
  {
    const IComponent *component = p.second;
    if (component)
    {
      this->_registerComponent(*component);
    }
  }
  for (auto &p : root.getChildren())
  {
    const Entity *child = p.second;
    if (child)
    {
      this->_visitSceneGraphRec(*child);
    }
  }
}

void Renderer::_registerComponent(const IComponent &component)
{
  ComponentType componentType = component.getTypeId();
  switch (componentType)
  {
  case ComponentType::DIRECTION_LIGHT:
  {
    this->_sceneContext.registerDirectionLight(*static_cast<const DirectionLight *>(&component),
                                               this->_sceneGraph, this->_shadowMappingShader);
  }
  break;
  case ComponentType::POINT_LIGHT:
  {
    this->_sceneContext.registerPointLight(*static_cast<const PointLight *>(&component),
                                           this->_sceneGraph, this->_cubeShadowMapShader);
  }
  break;
  case ComponentType::INSTANCED:
  {
  }
  break;
  case ComponentType::MATERIAL:
  {
    this->_sceneContext.registerMaterial(*static_cast<const Material *>(&component));
  }
  break;
  case ComponentType::TRANSFORMATION:
  {
  }
  break;
  case ComponentType::VOLUME:
  {
    this->_sceneContext.registerVolume(*static_cast<const Volume *>(&component));
  }
  break;
  default:
    break;
  }
}

void Renderer::render(const SceneGraph *sceneGraph)
{
  if (this->_cubeMapNode->getDependencies().size() == 0)
    this->_cubeMapNode->addDependency(this->_mainNode);
  if (this->_blitNode->getDependencies().size() == 0)
  {
    this->_blitNode->addDependency(this->_mainNode);
    this->_blitNode->addDependency(this->_cubeMapNode);
  }

  this->_renderGraph.push_back(this->_mainNode);
  this->_renderGraph.push_back(this->_cubeMapNode);
  this->_renderGraph.push_back(this->_postProcessNode);
  this->_renderGraph.push_back(this->_gammaCorrectionNode);
  this->_renderGraph.push_back(this->_blitNode);
  this->_renderGraph.push_back(this->_extractCapedBrightnessNode);
  this->_renderGraph.push_back(this->_blurNode);
  this->_renderGraph.push_back(this->_hdrCorrectionNode);
  this->_renderGraph.push_back(this->_bloomEffectNode);

  std::map<RenderGraphNode *, int> incompleteInputs;
  std::map<RenderGraphNode *, bool> hasRan;
  for (RenderGraphNode *node : this->_renderGraph)
  {
    incompleteInputs[node] = node->getInputs().size() + node->getDependencies().size();
    hasRan[node] = false;
  }

  for (auto &p : this->_sceneContext.dLights)
  {
    p.second.renderNode.render();
  }

  for (auto &p : this->_sceneContext.pLights)
  {
    p.second.renderNode.render();
  }

  this->_executeRenderGraph(incompleteInputs, hasRan);

  //this->_gBufferNode->render();
  //this->_deferredLightingNode->render();

  /*
  this->_mainNode->render();

  if (this->_instancedNode)
  {
    this->_instancedNode->render();
  }

  this->_cubeMapNode->render();

  this->_blitNode->render();

  this->_extractCapedBrightnessNode->render();

  this->_hdrCorrectionNode->render();

  this->_blurNode->render();

  this->_bloomEffectNode->render();

  this->_postProcessNode->render();

  this->_gammaCorrectionNode->render();
  */

  glfwSwapBuffers(this->_window);
}

void Renderer::_executeRenderGraph(std::map<RenderGraphNode *, int> &incompleteInputs, std::map<RenderGraphNode *, bool> &hasRan)
{
  bool change = false;
  bool finished = false;
  while (!finished)
  {
    finished = true;
    change = false;
    for (auto &p : incompleteInputs)
    {
      if (!hasRan[p.first])
      {
        if (p.second == 0)
        {
          p.first->render();
          change = true;
          hasRan[p.first] = true;
          std::vector<const TextureWrapper *> buffers;
          const Framebuffer *output = p.first->getOutput();
          if (output)
          {
            for (auto &tw : output->getColorBuffers())
            {
              const TextureWrapper *p_tw = &tw;
              buffers.push_back(p_tw);
            }
            for (auto &p2 : incompleteInputs)
            {
              for (auto &input : p2.first->getInputs())
              {
                if (std::find(buffers.begin(), buffers.end(), &input.second) != buffers.end())
                {
                  p2.second--;
                }
              }
              for (RenderGraphNode *dependency : p2.first->getDependencies())
              {
                if (dependency == p.first)
                {
                  p2.second--;
                }
              }
            }
          }
        }
        else
        {
          finished = false;
        }
      }
    }
    if (!finished && !change)
    {
      std::cerr << "Incomplete RenderGraph!" << std::endl;
      exit(1);
    }
  }
}

void Renderer::createMainNode(SceneGraph *sceneGraph)
{
  if (this->_mainNode == nullptr)
  {
    this->_mainNode = new MainNode(this->_context, this->_sceneContext, *sceneGraph, this->_shader, *this->_camera);
    this->_mainNode->getOutput() = &this->_multisampled;
    this->_gBufferNode = new MainNode(this->_context, this->_sceneContext, *sceneGraph, this->_gBufferShader, *this->_camera);
    this->_gBufferNode->getOutput() = &this->_gBuffer;
  }
}

void Renderer::createBlitNode()
{
  if (this->_blitNode == nullptr)
  {
    this->_blitNode = new BlitNode(this->_context, this->_multisampled);
    this->_blitNode->getOutput() = &this->_main;
  }
}

void Renderer::createInstancedNode(SceneGraph *sceneGraph, const std::vector<glm::mat4> &transformations)
{
  // Load instanced scene graph as well
  const Entity *root = sceneGraph->getRoot();
  if (root)
    this->_visitSceneGraphRec(*root);

  if (this->_instancedNode == nullptr)
  {
    this->_sceneContext.setInstancingVBO(transformations);
    this->_instancedNode = new InstancedNode(this->_context, this->_sceneContext, *sceneGraph, this->_instancingShader, *this->_camera, transformations);
    this->_instancedNode->getOutput() = &this->_multisampled;
  }
}

void Renderer::createCubeMapNode(SceneGraph *sceneGraph)
{
  if (this->_cubeMapNode == nullptr)
  {
    this->_cubeMapNode = new CubeMapNode(this->_context, this->_sceneContext, *sceneGraph, this->_cubeMapShader, *this->_camera);
    this->_cubeMapNode->getOutput() = &this->_multisampled;
  }
}

void Renderer::createPostProcessNode(SceneGraph *sceneGraph)
{
  if (this->_postProcessNode == nullptr)
  {
    this->_extractCapedBrightnessNode = new PostProcessNode(this->_context, this->_sceneContext, *sceneGraph, this->_extractCapedBrightnessShader);
    this->_extractCapedBrightnessNode->getInputs().insert(std::pair<std::string, const TextureWrapper &>("fb", this->_main.getColorBuffers()[0]));
    this->_extractCapedBrightnessNode->getOutput() = &this->_extractCapedBrightnessFB;

    this->_blurNode = new GaussianBlurNode(this->_context, this->_sceneContext, *sceneGraph);
    this->_blurNode->getInputs().insert(std::pair<std::string, const TextureWrapper &>("fb", this->_extractCapedBrightnessFB.getColorBuffers()[1]));
    this->_blurNode->getOutput() = &this->_blurFB;

    this->_hdrCorrectionNode = new PostProcessNode(this->_context, this->_sceneContext, *sceneGraph, this->_hdrCorrectionShader);
    this->_hdrCorrectionNode->getInputs().insert(std::pair<std::string, const TextureWrapper &>("fb", this->_extractCapedBrightnessFB.getColorBuffers()[0]));
    this->_hdrCorrectionNode->getOutput() = &this->_hdrCorrectionFB;

    this->_bloomEffectNode = new PostProcessNode(this->_context, this->_sceneContext, *sceneGraph, this->_bloomEffectShader);
    this->_bloomEffectNode->getInputs().insert(std::pair<std::string, const TextureWrapper &>("fb0", this->_hdrCorrectionFB.getColorBuffers()[0]));
    this->_bloomEffectNode->getInputs().insert(std::pair<std::string, const TextureWrapper &>("fb1", this->_blurFB.getColorBuffers()[0]));
    this->_bloomEffectNode->getOutput() = &this->_bloomEffectFB;

    this->_postProcessNode = new PostProcessNode(this->_context, this->_sceneContext, *sceneGraph, this->_postProcessShader);
    this->_postProcessNode->getInputs().insert(std::pair<std::string, const TextureWrapper &>("fb", this->_bloomEffectFB.getColorBuffers()[0]));
    this->_postProcessNode->getOutput() = &this->_postProcess;

    this->_deferredLightingNode = new DeferredLightingNode(this->_context, this->_sceneContext, *sceneGraph, this->_deferredLightingShader, *this->_camera);
    this->_deferredLightingNode->getInputs().insert(std::pair<std::string, const TextureWrapper &>("fb0", this->_gBuffer.getColorBuffers()[0]));
    this->_deferredLightingNode->getInputs().insert(std::pair<std::string, const TextureWrapper &>("fb1", this->_gBuffer.getColorBuffers()[1]));
    this->_deferredLightingNode->getInputs().insert(std::pair<std::string, const TextureWrapper &>("fb2", this->_gBuffer.getColorBuffers()[2]));
    this->_deferredLightingNode->getInputs().insert(std::pair<std::string, const TextureWrapper &>("fb3", this->_gBuffer.getColorBuffers()[3]));
  }
}

void Renderer::createGammaCorrectionNode(SceneGraph *sceneGraph)
{
  if (this->_gammaCorrectionNode == nullptr)
  {
    this->_gammaCorrectionNode = new PostProcessNode(this->_context, this->_sceneContext, *sceneGraph, this->_gammaCorrectionShader);
    this->_gammaCorrectionNode->getInputs().insert(std::pair<std::string, const TextureWrapper &>("fb", this->_postProcess.getColorBuffers()[0]));
  }
}

void Renderer::notified(Subject *subject, Event event)
{
  IComponent *c = dynamic_cast<IComponent *>(subject);
  if (c)
  {
    // TODO: Separate events crated, updated, deleted etc. But for now lets focus on batch mode
    this->_registerComponent(*c);
    return;
  }
  Entity *e = dynamic_cast<Entity *>(subject);
  if (e)
  {
    // TODO: Separate events crated, updated, deleted etc. But for now lets focus on batch mode
    this->_visitSceneGraphRec(*e);
    return;
  }
}

} // namespace leo
