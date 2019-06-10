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

Renderer::Renderer()
{
  this->_init();
}

Renderer::~Renderer()
{
  if (_mainNode != nullptr)
    delete _mainNode;
}

void Renderer::_init()
{
  this->_initFramebuffers();
  //this->_visitSceneGraph();
}

void Renderer::_initFramebuffers()
{
  /*
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
  */
}

void Renderer::_setWindowContext(GLFWwindow *window, InputManager *inputManager)
{
  this->_inputManager = inputManager;
  this->_window = window;
  //this->_context.setWindowContext(*window, *inputManager);
}

void Renderer::_setCamera(Camera *camera)
{
  this->_camera = camera;
}

void Renderer::_visitSceneGraph()
{
  const Entity *root = this->_sceneGraph->getRoot();
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
    this->_sceneContext->registerDirectionLight(*static_cast<const DirectionLight *>(&component),
                                                *this->_sceneGraph, this->_shadowMappingShader);
  }
  break;
  case ComponentType::POINT_LIGHT:
  {
    this->_sceneContext->registerPointLight(*static_cast<const PointLight *>(&component),
                                            *this->_sceneGraph, this->_cubeShadowMapShader);
  }
  break;
  case ComponentType::INSTANCED:
  {
  }
  break;
  case ComponentType::MATERIAL:
  {
    this->_sceneContext->registerMaterial(*static_cast<const Material *>(&component));
  }
  break;
  case ComponentType::TRANSFORMATION:
  {
  }
  break;
  case ComponentType::VOLUME:
  {
    this->_sceneContext->registerVolume(*static_cast<const Volume *>(&component));
  }
  break;
  default:
    break;
  }
}

void Renderer::createSceneContext(OpenGLContext &context) // TODO: Remove, create a separate class to manage observation
{
  this->_sceneContext = new SceneContext(context);
}

void Renderer::setSceneGraph(SceneGraph &sceneGraph) // TODO: Remove, create a separate class to manage observation
{
  this->_sceneGraph = &sceneGraph;
  this->_sceneGraph->watch(this);
  this->_visitSceneGraph();
}

void Renderer::render(const SceneGraph *sceneGraph)
{
  if (this->_cubeMapNode->getInNodes().size() == 0)
    this->_cubeMapNode->addInNode(*this->_mainNode);
  if (this->_blitNode->getInNodes().size() == 0)
  {
    this->_blitNode->addInNode(*this->_mainNode);
    this->_blitNode->addInNode(*this->_cubeMapNode);
  }

  /*
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
    incompleteInputs[node] = node->getInNodes().size();
    hasRan[node] = false;
  }
  */

  /*
  for (auto &p : this->_sceneContext.dLights)
  {
    p.second.renderNode.render();
  }

  for (auto &p : this->_sceneContext.pLights)
  {
    p.second.renderNode.render();
  }
*/

  //this->_executeRenderGraph(incompleteInputs, hasRan);

  //this->_gBufferNode->render();
  //this->_deferredLightingNode->render();

  this->_renderGraph.execute();

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

/*
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
          const Framebuffer *output = p.first->getFramebuffer();
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
              for (t_RGNodeId dependency : p2.first->getInNodes())
              {
                if (dependency == p.first->getId())
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
*/

void Renderer::createMainNode(SceneGraph *sceneGraph)
{
  if (this->_mainNode == nullptr)
  {
    /*
    this->_mainNode = this->_renderGraph.createNode<MainNode>(this->_context, this->_sceneContext, *sceneGraph, this->_shader, *this->_camera);
    this->_mainNode->setFramebuffer(&this->_multisampled);
    this->_gBufferNode = this->_renderGraph.createNode<MainNode>(this->_context, this->_sceneContext, *sceneGraph, this->_gBufferShader, *this->_camera);
    this->_gBufferNode->setFramebuffer(&this->_gBuffer)
    */
    ;
  }
}

void Renderer::createBlitNode()
{
  if (this->_blitNode == nullptr)
  {
    /*
    this->_blitNode = this->_renderGraph.createNode<BlitNode>(this->_context, this->_multisampled);
    this->_blitNode->setFramebuffer(&this->_main);
    */
  }
}

void Renderer::createInstancedNode(SceneGraph *sceneGraph, const std::vector<glm::mat4> &transformations)
{
  // Load instanced scene graph as well
  /*
  const Entity *root = sceneGraph->getRoot();
  if (root)
    this->_visitSceneGraphRec(*root);

  if (this->_instancedNode == nullptr)
  {
    this->_sceneContext.setInstancingVBO(transformations);
    this->_instancedNode = this->_renderGraph.createNode<InstancedNode>(this->_context, this->_sceneContext, *sceneGraph, this->_instancingShader, *this->_camera, transformations);
    this->_instancedNode->setFramebuffer(&this->_multisampled);
  }
  */
}

void Renderer::createCubeMapNode(SceneGraph *sceneGraph)
{
  /*
  if (this->_cubeMapNode == nullptr)
  {
    this->_cubeMapNode = this->_renderGraph.createNode<CubeMapNode>(this->_context, this->_sceneContext, *sceneGraph, this->_cubeMapShader, *this->_camera);
    this->_cubeMapNode->setFramebuffer(&this->_multisampled);
  }
  */
}

void Renderer::createPostProcessNode(SceneGraph *sceneGraph)
{
  /*
  if (this->_postProcessNode == nullptr)
  {
    this->_extractCapedBrightnessNode = this->_renderGraph.createNode<PostProcessNode>(this->_context, this->_sceneContext, *sceneGraph, this->_extractCapedBrightnessShader);
    this->_extractCapedBrightnessNode->addInput(*this->_blitNode, "fb", 0);
    this->_extractCapedBrightnessNode->setFramebuffer(&this->_extractCapedBrightnessFB);

    this->_blurNode = this->_renderGraph.createNode<GaussianBlurNode>(this->_context, this->_sceneContext, *sceneGraph);
    this->_blurNode->addInput(*this->_extractCapedBrightnessNode, "fb", 1);
    this->_blurNode->setFramebuffer(&this->_blurFB);

    this->_hdrCorrectionNode = this->_renderGraph.createNode<PostProcessNode>(this->_context, this->_sceneContext, *sceneGraph, this->_hdrCorrectionShader);
    this->_hdrCorrectionNode->addInput(*this->_extractCapedBrightnessNode, "fb", 0);
    this->_hdrCorrectionNode->setFramebuffer(&this->_hdrCorrectionFB);

    this->_bloomEffectNode = this->_renderGraph.createNode<PostProcessNode>(this->_context, this->_sceneContext, *sceneGraph, this->_bloomEffectShader);
    this->_bloomEffectNode->addInput(*this->_hdrCorrectionNode, "fb0", 0);
    this->_bloomEffectNode->addInput(*this->_blurNode, "fb1", 0);
    this->_bloomEffectNode->setFramebuffer(&this->_bloomEffectFB);

    this->_postProcessNode = this->_renderGraph.createNode<PostProcessNode>(this->_context, this->_sceneContext, *sceneGraph, this->_postProcessShader);
    this->_postProcessNode->addInput(*this->_bloomEffectNode, "fb", 0);
    this->_postProcessNode->setFramebuffer(&this->_postProcess);

    this->_deferredLightingNode = this->_renderGraph.createNode<DeferredLightingNode>(this->_context, this->_sceneContext, *sceneGraph, this->_deferredLightingShader, *this->_camera);
    this->_deferredLightingNode->addInput(*this->_gBufferNode, "fb0", 0);
    this->_deferredLightingNode->addInput(*this->_gBufferNode, "fb1", 1);
    this->_deferredLightingNode->addInput(*this->_gBufferNode, "fb2", 2);
    this->_deferredLightingNode->addInput(*this->_gBufferNode, "fb3", 3);
  }
  */
}

void Renderer::createGammaCorrectionNode(SceneGraph *sceneGraph)
{
  /*
  if (this->_gammaCorrectionNode == nullptr)
  {
    this->_gammaCorrectionNode = this->_renderGraph.createNode<PostProcessNode>(this->_context, this->_sceneContext, *sceneGraph, this->_gammaCorrectionShader);
    this->_gammaCorrectionNode->addInput(*this->_postProcessNode, "fb", 0);
  }
  */
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
