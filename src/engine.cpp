#include "engine.hpp"

namespace leo {


Engine::Engine() {
  this->_init();
}

Engine::~Engine() {
  delete this->_camera;
  delete this->_root;
}

void Engine::_init() {
  // Init GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  this->_window = glfwCreateWindow(this->screenWidth, this->screenHeight, "OpenGL", nullptr, nullptr);
  this->_camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
  glfwSetWindowUserPointer(this->_window, &this->inputManager);
  glfwMakeContextCurrent(this->_window);

  // Set the required callback functions
  glfwSetKeyCallback(this->_window, this->keyCallback);
  glfwSetCursorPosCallback(this->_window, this->mouseCallback);

  // Options
  glfwSetInputMode(this->_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSwapInterval(1);

  // Initialize GLEW to setup the OpenGL Function pointers
  glewExperimental = GL_TRUE;

  // Define the viewport dimensions
  glClearColor(0.07, 0.07, 0.07, 1);

  // Setup some OpenGL options
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS); // Set to always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))


  if (glewInit() == GLEW_OK) {
    std::cerr << "Glew initialized successfully" << std::endl;
  }
  else {
    std::cerr << "Failed to initialize Glew" << std::endl;
    return;
  }

  // Initialize scene graph
  // TODO: create FolderNode
  this->_root = new CubeGeometry();

}

void Engine::gameLoop() {
  RenderVisitor render_visitor(this->_camera, this->_window);
  GLfloat lastFrame = 0.0;
  GLfloat deltaTime = 0.0;
  GLfloat currentFrame = 0.0;

  while(!glfwWindowShouldClose(this->_window)) {
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    this->doMovement(deltaTime);

    glViewport(0, 0, screenWidth, screenHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_visitor.visit(this->_root);

    glfwSwapBuffers(this->_window);

    // Check and call events
    glfwPollEvents();
  }

  glfwDestroyWindow(this->_window);
  glfwTerminate();
}


Node *Engine::getRoot() {
  return this->_root;
}

// Moves/alters the camera positions based on user input
void Engine::doMovement(float deltaTime)
{
  // Keys controls
  if(this->inputManager.keys[GLFW_KEY_W])
    this->_camera->processKeyboard(FORWARD, deltaTime);
  if(this->inputManager.keys[GLFW_KEY_S])
    this->_camera->processKeyboard(BACKWARD, deltaTime);
  if(this->inputManager.keys[GLFW_KEY_A])
    this->_camera->processKeyboard(LEFT, deltaTime);
  if(this->inputManager.keys[GLFW_KEY_D])
    this->_camera->processKeyboard(RIGHT, deltaTime);
  if(this->inputManager.keys[GLFW_KEY_LEFT_SHIFT])
    this->_camera->processKeyboard(DOWN, deltaTime);
  if(this->inputManager.keys[GLFW_KEY_SPACE])
    this->_camera->processKeyboard(UP, deltaTime);

  // Mouse controls
  GLfloat xo = this->inputManager.pollXOffset();
  GLfloat yo = this->inputManager.pollYOffset();
  if (xo != 0 || yo != 0) {
    this->_camera->processMouse(xo, yo);
  }
}

// Is called whenever a key is pressed/released via GLFW
void Engine::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  UNUSED(scancode); UNUSED(mode);
  InputManager *im = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (key >= 0 && key < 1024)
  {
    if(action == GLFW_PRESS)
      im->pressKey(key);
    else if(action == GLFW_RELEASE)
      im->releaseKey(key);
  }
}

void Engine::mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
  InputManager *im = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
  if(im->firstMouse) {
    im->setLastX(xpos);
    im->setLastY(ypos);
    im->setFirstMouse(false);
  }
  GLfloat xoffset = xpos - im->lastX;
  GLfloat yoffset = im->lastY - ypos;  // Reversed since y-coordinates go from bottom to left
  im->setLastX(xpos);
  im->setLastY(ypos);
  im->setXOffset(xoffset);
  im->setYOffset(yoffset);
}

}
