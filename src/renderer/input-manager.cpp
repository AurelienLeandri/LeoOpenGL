#include "input-manager.hpp"

namespace leo {
  std::shared_ptr<InputManager> InputManager::_instance = std::shared_ptr<InputManager>(nullptr);

  InputManager::InputManager() {
  }

  InputManager *InputManager::getInstance() {
    if (!_instance)
      _instance = std::make_shared<InputManager>(InputManager());
    return _instance.get();
  }

  void InputManager::pressKey(int key) {
    _instance->keys[key] = true;
  }

  void InputManager::releaseKey(int key) {
    _instance->keys[key] = false;
  }

  void InputManager::setLastX(GLfloat val) {
    _instance->lastX = val;
  }

  void InputManager::setLastY(GLfloat val) {
    _instance->lastY = val;
  }

  void InputManager::setFirstMouse(bool val) {
    _instance->firstMouse = val;
  }

  void InputManager::setXOffset(GLfloat val) {
    _instance->xOffset = val;
  }

  void InputManager::setYOffset(GLfloat val) {
    _instance->yOffset = val;
  }

  GLfloat InputManager::pollXOffset() {
    GLfloat val = _instance->xOffset;
    _instance->xOffset = 0;
    return val;
  }

  GLfloat InputManager::pollYOffset() {
    GLfloat val = _instance->yOffset;
    _instance->yOffset = 0;
    return val;
  }

  // Is called whenever a key is pressed/released via GLFW
  void InputManager::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
  {
    UNUSED(scancode); UNUSED(mode);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
      if (action == GLFW_PRESS)
        _instance->pressKey(key);
      else if (action == GLFW_RELEASE)
        _instance->releaseKey(key);
    }
  }

  void InputManager::mouseCallback(GLFWwindow *window, double xpos, double ypos)
  {
    if (_instance->firstMouse) {
      _instance->setLastX(xpos);
      _instance->setLastY(ypos);
      _instance->setFirstMouse(false);
    }
    GLfloat xoffset = xpos - _instance->lastX;
    GLfloat yoffset = _instance->lastY - ypos;  // Reversed since y-coordinates go from bottom to left
    _instance->setLastX(xpos);
    _instance->setLastY(ypos);
    _instance->setXOffset(xoffset);
    _instance->setYOffset(yoffset);
  }

}
