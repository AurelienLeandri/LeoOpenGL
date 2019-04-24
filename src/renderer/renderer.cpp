#include "renderer.hpp"

#include <utils/texture.hpp>

#include <renderer/input-manager.hpp>
#include <renderer/camera.hpp>
#include <renderer/debug.hpp>
#include <renderer/main-node.hpp>
#include <renderer/cube-map-node.hpp>
#include <renderer/post-process-node.hpp>
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

namespace leo
{

Renderer::Renderer(GLFWwindow *window,
                   InputManager *inputManager,
                   Camera *camera,
                   Shader shader,
                   const SceneGraph &sceneGraph) : _shader(shader), _sceneGraph(sceneGraph),
                                                   _postProcessShader("resources/shaders/post-process.vertex.glsl", "resources/shaders/post-process.fragment.glsl"),
                                                   _cubeMapShader("resources/shaders/cube-map.vs.glsl", "resources/shaders/cube-map.frag.glsl"),
                                                   _instancingShader("resources/shaders/instancing.vs.glsl", "resources/shaders/basic.frag.glsl"),
                                                   _gammaCorrectionShader("resources/shaders/post-process.vertex.glsl", "resources/shaders/gamma-correction.frag.glsl"),
                                                   _multisampled({true, 4}),
                                                   _blitNode(this->_context),
                                                   _shadowMappingShader("resources/shaders/dir-shadow-mapping.vs.glsl", "resources/shaders/dir-shadow-mapping.frag.glsl"),
                                                   _sceneContext(this->_context)

{
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
  this->_main.generate();
  this->_multisampled.generate();
  this->_postProcess.generate();
  this->_blitNode.setOutput(&this->_main);
  this->_blitNode.getInputs().insert(std::pair<std::string, Framebuffer *>("in", &this->_multisampled));
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
    this->_sceneContext.registerPointLight(*static_cast<const PointLight *>(&component));
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

  for (auto &p : this->_sceneContext.dLights)
  {
    p.second.renderNode.render();
  }

  this->_mainNode->render();

  if (this->_instancedNode)
  {
    this->_instancedNode->render();
  }

  this->_cubeMapNode->render();

  this->_blitNode.render();

  this->_postProcessNode->render();

  this->_gammaCorrectionNode->render();

  glfwSwapBuffers(this->_window);
}

void Renderer::createMainNode(SceneGraph *sceneGraph)
{
  if (this->_mainNode == nullptr)
  {
    this->_mainNode = new MainNode(this->_context, this->_sceneContext, *sceneGraph, this->_shader, *this->_camera);
    this->_mainNode->setOutput(&this->_multisampled);
  }
  std::vector<Observer *> obs;
  obs.push_back(this->_mainNode);
  sceneGraph->reloadScene(obs);
}

void Renderer::createInstancedNode(SceneGraph *sceneGraph, const std::vector<glm::mat4> &transformations)
{
  if (this->_instancedNode == nullptr)
  {
    this->_sceneContext.setInstancingVBO(transformations);
    this->_instancedNode = new InstancedNode(this->_context, this->_sceneContext, *sceneGraph, this->_instancingShader, *this->_camera, transformations);
    this->_instancedNode->setOutput(&this->_multisampled);
  }
  std::vector<Observer *> obs;
  obs.push_back(this->_instancedNode);
  sceneGraph->reloadScene(obs);
}

void Renderer::createCubeMapNode(SceneGraph *sceneGraph)
{
  if (this->_cubeMapNode == nullptr)
  {
    this->_cubeMapNode = new CubeMapNode(this->_context, this->_sceneContext, *sceneGraph, this->_cubeMapShader, *this->_camera);
    this->_cubeMapNode->setOutput(&this->_multisampled);
  }
}

void Renderer::createPostProcessNode(SceneGraph *sceneGraph)
{
  if (this->_postProcessNode == nullptr)
  {
    this->_postProcessNode = new PostProcessNode(this->_context, this->_sceneContext, *sceneGraph, this->_postProcessShader);
    this->_postProcessNode->getInputs().insert(std::pair<std::string, Framebuffer *>("fb", &this->_main));
    this->_postProcessNode->setOutput(&this->_postProcess);
  }
}

void Renderer::createGammaCorrectionNode(SceneGraph *sceneGraph)
{
  if (this->_gammaCorrectionNode == nullptr)
  {
    this->_gammaCorrectionNode = new PostProcessNode(this->_context, this->_sceneContext, *sceneGraph, this->_gammaCorrectionShader);
    this->_gammaCorrectionNode->getInputs().insert(std::pair<std::string, Framebuffer *>("fb", &this->_postProcess));
    this->_gammaCorrectionNode->setOutput(nullptr);
  }
}

void Renderer::notified(Subject *subject, Event event)
{
  IComponent *c = dynamic_cast<IComponent *>(subject);
  if (c)
  {
    if (event == Event::COMPONENT_ADDED)
      this->_registerComponent(*c);
  }
}

} // namespace leo
