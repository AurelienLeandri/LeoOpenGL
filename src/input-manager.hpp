#pragma once

#include <global.hpp>

#include <vector>
#include <mutex>

namespace leo {

  class InputManager {
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

    public:
      GLfloat lastX = 400;
      GLfloat lastY = 300;
      GLfloat xOffset = 0;
      GLfloat yOffset = 0;
      bool firstMouse = true;
      bool keys[1024] = {false};
  };

}
