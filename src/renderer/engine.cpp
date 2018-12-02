#include "engine.hpp"

#include <renderer/scene/visitor/transformation-visitor.hpp>
#include <renderer/scene/lights/point-light.hpp>
#include <renderer/input-manager.hpp>

namespace leo {


  Engine::Engine() {
    this->_init();
  }

  Engine::~Engine() {
    delete this->_camera;
    delete this->_renderer;
  }

  void Engine::_init() {
    this->_window = glfwCreateWindow(this->screenWidth, this->screenHeight, " ~~~ LeoEngine!!", nullptr, nullptr);
    this->_camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    this->inputManager = InputManager::getInstance();
    this->_renderer = new renderer::Renderer();
    this->_renderer->setWindowContext(this->_window, this->inputManager);
    this->_renderer->setCamera(this->_camera);
  }

  void Engine::gameLoop() {
    // Render to our framebuffer
    GLfloat lastFrame = 0.0;
    GLfloat deltaTime = 0.0;
    GLfloat currentFrame = 0.0;

    while(!glfwWindowShouldClose(this->_window)) {
      currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      this->doMovement(deltaTime);

      this->_renderer->render(nullptr);

      // Check and call events
      glfwPollEvents();
    }

    glfwDestroyWindow(this->_window);
    glfwTerminate();
  }

  // Moves/alters the camera positions based on user input
  void Engine::doMovement(float deltaTime)
  {
    // Keys controls
    if(this->inputManager->keys[GLFW_KEY_W])
      this->_camera->processKeyboard(FORWARD, deltaTime);
    if(this->inputManager->keys[GLFW_KEY_S])
      this->_camera->processKeyboard(BACKWARD, deltaTime);
    if(this->inputManager->keys[GLFW_KEY_A])
      this->_camera->processKeyboard(LEFT, deltaTime);
    if(this->inputManager->keys[GLFW_KEY_D])
      this->_camera->processKeyboard(RIGHT, deltaTime);
    if(this->inputManager->keys[GLFW_KEY_LEFT_SHIFT])
      this->_camera->processKeyboard(DOWN, deltaTime);
    if(this->inputManager->keys[GLFW_KEY_SPACE])
      this->_camera->processKeyboard(UP, deltaTime);

    // Mouse controls
    GLfloat xo = this->inputManager->pollXOffset();
    GLfloat yo = this->inputManager->pollYOffset();
    if (xo != 0 || yo != 0) {
      this->_camera->processMouse(xo, yo);
    }
  }

}
