#pragma once

#include <renderer/global.hpp>

#include <memory>

namespace leo
{

namespace renderer
{

class InputManager
{
public:
  static InputManager *getInstance();

public:
  void pressKey(int key);
  void releaseKey(int key);
  void setLastX(GLfloat val);
  void setXOffset(GLfloat val);
  void setLastY(GLfloat val);
  void setYOffset(GLfloat val);
  void setFirstMouse(bool val);
  GLfloat pollYOffset();
  GLfloat pollXOffset();

private:
  InputManager();

private:
  static std::shared_ptr<InputManager> _instance;

public:
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
  static void mouseCallback(GLFWwindow *window, double xpos, double ypos);

public:
  GLfloat lastX = 400;
  GLfloat lastY = 300;
  GLfloat xOffset = 0;
  GLfloat yOffset = 0;
  bool firstMouse = true;
  bool keys[1024] = {false};
};

} // namespace renderer

} // namespace leo
