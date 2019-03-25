#include "renderer.hpp"

#include <utils/texture.hpp>

#include <renderer/input-manager.hpp>
#include <renderer/camera.hpp>
#include <renderer/debug.hpp>
#include <renderer/main-node.hpp>
#include <renderer/cube-map-node.hpp>
#include <renderer/post-process-node.hpp>
#include <renderer/instanced-node.hpp>

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
                                    _instancingShader("resources/shaders/instancing.vs.glsl", "resources/shaders/basic.frag.glsl")
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

  this->_mainNode->render();

  if (this->_instancedNode)
  {
    this->_instancedNode->render();
  }

  this->_cubeMapNode->render();
  //this->_postProcessNode->render();

  glfwSwapBuffers(this->_window);
}

void Renderer::createMainNode(SceneGraph *sceneGraph)
{
  if (this->_mainNode == nullptr)
  {
    this->_mainNode = new MainNode(this->_context, *sceneGraph, this->_shader, *this->_camera);
    //this->_mainNode->setOutput(&this->_main);
    this->_mainNode->setOutput(nullptr);
    this->_mainNode->enableMultiSampling();
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
    this->_instancedNode->setOutput(&this->_main);
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
    this->_cubeMapNode->setOutput(&this->_main);
    //this->_cubeMapNode->setOutput(nullptr);
  }
}

void Renderer::createPostProcessNode(SceneGraph *sceneGraph)
{
  if (this->_postProcessNode == nullptr)
  {
    this->_postProcessNode = new PostProcessNode(this->_context, *sceneGraph, this->_postProcessShader, *this->_camera);
    auto &inputs = this->_postProcessNode->getInputs();
    inputs.insert(std::pair<std::string, Framebuffer *>("fb", &this->_main));
    this->_postProcessNode->setOutput(nullptr);
  }
}

/////////////////////////////////////
/*        LOADING FUNCTIONS        */
/////////////////////////////////////

void Renderer::_loadInstanced(const Instanced *instanced)
{
  const std::vector<glm::mat4> &m = instanced->transformations;
  int amount = m.size();

  // vertex Buffer Object
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &m[0], GL_STATIC_DRAW);

  std::vector<const Volume *> meshes;
  this->_getChildrenMeshes(instanced->getEntity(), meshes);

  for (unsigned int i = 0; i < meshes.size(); i++)
  {
    //this->_context.loadVAOInstanced(*meshes[i]);
  }
}

void Renderer::_getChildrenMeshes(const Entity *root, std::vector<const Volume *> &meshes)
{
  const Volume *v = static_cast<const Volume *>(root->getComponent(ComponentType::VOLUME));
  if (v)
  {
    this->_context.loadVAO(*v);
    meshes.push_back(v);
  }
  for (auto p : root->getChildren())
  {
    this->_getChildrenMeshes(p.second, meshes);
  }
}

} // namespace leo
