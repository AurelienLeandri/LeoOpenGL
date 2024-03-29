#include "engine.hpp"

#include <renderer/renderer.hpp>
#include <renderer/camera.hpp>
#include <renderer/input-manager.hpp>
#include <model/scene-graph.hpp>

namespace leo
{

Engine::Engine()
{
  this->_init();
}

Engine::~Engine()
{
  delete this->_camera;
  if (this->_renderer)
    delete this->_renderer;
}

void Engine::_init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  //glfwWindowHint(GLFW_SAMPLES, 4);

  glfwSwapInterval(0);

  this->_window = glfwCreateWindow(this->screenWidth, this->screenHeight, " ~~~ LeoEngine!!", nullptr, nullptr);

  if (this->_window == NULL)
  {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(1);
  }
  glfwMakeContextCurrent(this->_window);

  this->_camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
  this->inputManager = InputManager::getInstance();
}

void Engine::_initRenderer(Shader shader)
{
  if (this->_scene)
  {
    this->_renderer = new Renderer(
        this->_window,
        this->inputManager,
        this->_camera,
        shader,
        *this->_scene);
    this->_renderer->createMainNode(this->_scene);
    this->_renderer->createBlitNode();
    this->_renderer->createCubeMapNode(this->_scene);
    this->_renderer->createPostProcessNode(this->_scene);
    this->_renderer->createGammaCorrectionNode(this->_scene);
  }
}

void Engine::setScene(SceneGraph *scene)
{
  this->_scene = scene;
  if (!this->_renderer)
  {
    Shader shader(
        "resources/shaders/basic.vs.glsl",
        "resources/shaders/basic.frag.glsl");
    this->_initRenderer(shader);
  }
}

void Engine::setInstancedScene(SceneGraph *scene, const std::vector<glm::mat4> &transformations)
{
  this->_renderer->createInstancedNode(scene, transformations);
}

void Engine::gameLoop()
{
  // Render to our framebuffer
  GLfloat lastFrame = 0.0;
  GLfloat deltaTime = 0.0;
  GLfloat currentFrame = 0.0;

  while (!glfwWindowShouldClose(this->_window))
  {
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    this->doMovement(deltaTime);

    if (this->_scene)
    {
      this->_renderer->render(this->_scene);
    }

    // Check and call events
    glfwPollEvents();
  }

  glfwDestroyWindow(this->_window);
  glfwTerminate();
}

// Moves/alters the camera positions entityd on user input
void Engine::doMovement(float deltaTime)
{
  // Keys controls
  if (this->inputManager->keys[GLFW_KEY_W])
    this->_camera->processKeyboard(FORWARD, deltaTime);
  if (this->inputManager->keys[GLFW_KEY_S])
    this->_camera->processKeyboard(BACKWARD, deltaTime);
  if (this->inputManager->keys[GLFW_KEY_A])
    this->_camera->processKeyboard(LEFT, deltaTime);
  if (this->inputManager->keys[GLFW_KEY_D])
    this->_camera->processKeyboard(RIGHT, deltaTime);
  if (this->inputManager->keys[GLFW_KEY_LEFT_SHIFT])
    this->_camera->processKeyboard(DOWN, deltaTime);
  if (this->inputManager->keys[GLFW_KEY_SPACE])
    this->_camera->processKeyboard(UP, deltaTime);

  // Mouse controls
  GLfloat xo = this->inputManager->pollXOffset();
  GLfloat yo = this->inputManager->pollYOffset();
  if (xo != 0 || yo != 0)
  {
    this->_camera->processMouse(xo, yo);
  }
}

} // namespace leo
