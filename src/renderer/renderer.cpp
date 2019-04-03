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
                   Shader shader) : _shader(shader), _postProcessShader("resources/shaders/post-process.vertex.glsl", "resources/shaders/post-process.fragment.glsl"),
                                    _cubeMapShader("resources/shaders/cube-map.vs.glsl", "resources/shaders/cube-map.frag.glsl"),
                                    _instancingShader("resources/shaders/instancing.vs.glsl", "resources/shaders/basic.frag.glsl"),
                                    _gammaCorrectionShader("resources/shaders/post-process.vertex.glsl", "resources/shaders/gamma-correction.frag.glsl"),
                                    _multisampled({true, 4}),
                                    _blitNode(this->_context),
                                    _shadowMappingShader("resources/shaders/dir-shadow-mapping.vs.glsl", "resources/shaders/dir-shadow-mapping.frag.glsl")

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

void Renderer::render(const SceneGraph *sceneGraph)
{

  for (auto &p : this->_directionalShadowNodes)
  {
    p.second.render();
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
    this->_mainNode = new MainNode(this->_context, *sceneGraph, this->_shader, *this->_camera);
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
    this->_instancedNode = new InstancedNode(this->_context, *sceneGraph, this->_instancingShader, *this->_camera, transformations);
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
    this->_cubeMapNode = new CubeMapNode(this->_context, *sceneGraph, this->_cubeMapShader, *this->_camera);
    this->_cubeMapNode->setOutput(&this->_multisampled);
  }
}

void Renderer::createPostProcessNode(SceneGraph *sceneGraph)
{
  if (this->_postProcessNode == nullptr)
  {
    this->_postProcessNode = new PostProcessNode(this->_context, *sceneGraph, this->_postProcessShader);
    this->_postProcessNode->getInputs().insert(std::pair<std::string, Framebuffer *>("fb", &this->_main));
    this->_postProcessNode->setOutput(&this->_postProcess);
  }
}

void Renderer::createGammaCorrectionNode(SceneGraph *sceneGraph)
{
  if (this->_gammaCorrectionNode == nullptr)
  {
    this->_gammaCorrectionNode = new PostProcessNode(this->_context, *sceneGraph, this->_gammaCorrectionShader);
    this->_gammaCorrectionNode->getInputs().insert(std::pair<std::string, Framebuffer *>("fb", &this->_postProcess));
    this->_gammaCorrectionNode->setOutput(nullptr);
  }
}

void Renderer::setShadowSceneGraph(SceneGraph &sceneGraph)
{
  this->_shadowSceneGraph = &sceneGraph;
  this->_shadowSceneGraph->watch(this);
  std::vector<Observer *> obs;
  obs.push_back(this);
  sceneGraph.reloadScene(obs);
}

void Renderer::notified(Subject *subject, Event event)
{
  DirectionLight *c = dynamic_cast<DirectionLight *>(subject);
  if (c)
  {
    ShadowMappingNode &shadowMap = this->_directionalShadowNodes.insert(std::pair<t_id, ShadowMappingNode>(
                                                                            c->getId(),
                                                                            ShadowMappingNode(this->_context, *this->_shadowSceneGraph, this->_shadowMappingShader, *c)))
                                       .first->second;
    FramebufferOptions options;
    options.width = 1620 * 2;
    options.height = 1080 * 2;
    options.type = FrameBufferType::DEPTH_MAP;
    Framebuffer &fbOut = this->_directionalShadowMaps.insert(std::pair<t_id, Framebuffer>(
                                                                 c->getId(),
                                                                 Framebuffer(options)))
                             .first->second;
    fbOut.generate();
    shadowMap.setOutput(&fbOut);
    this->_mainNode->getInputs().insert(std::pair<std::string, Framebuffer *>("shadowMap", &fbOut));
  }
}

} // namespace leo
