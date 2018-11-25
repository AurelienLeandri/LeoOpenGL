#include "input-manager.hpp"

namespace leo {

  void InputManager::pressKey(int key) {
    this->keys[key] = true;
  }

  void InputManager::releaseKey(int key) {
    this->keys[key] = false;
  }

  void InputManager::setLastX(GLfloat val) {
    this->lastX = val;
  }

  void InputManager::setLastY(GLfloat val) {
    this->lastY = val;
  }

  void InputManager::setFirstMouse(bool val) {
    this->firstMouse = val;
  }

  void InputManager::setXOffset(GLfloat val) {
    this->xOffset = val;
  }

  void InputManager::setYOffset(GLfloat val) {
    this->yOffset = val;
  }

  GLfloat InputManager::pollXOffset() {
    GLfloat val = this->xOffset;
    this->xOffset = 0;
    return val;
  }

  GLfloat InputManager::pollYOffset() {
    GLfloat val = this->yOffset;
    this->yOffset = 0;
    return val;
  }

}
