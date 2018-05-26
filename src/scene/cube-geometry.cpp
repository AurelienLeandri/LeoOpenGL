#include "cube-geometry.hpp"
#include <glm/gtx/transform.hpp>

namespace leo {

CubeGeometry::CubeGeometry() : CubeGeometry(glm::vec3(0, 0, 0), 1.0) {
}

CubeGeometry::CubeGeometry(float size)
  : CubeGeometry(glm::vec3(0, 0, 0), size)
{
}

CubeGeometry::CubeGeometry(glm::vec3 position, float size) {
  this->_data = std::vector<GLfloat>{
      size, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
      size,  size, 0.0f, 0.0f, 0.0f, -1.0f,
      0.0f,  size, 0.0f, 0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

      size, 0.0f,  size, 0.0f, 0.0f, 1.0f,
      size,  size,  size, 0.0f, 0.0f, 1.0f,
      0.0f,  size,  size, 0.0f, 0.0f, 1.0f,
      0.0f, 0.0f,  size, 0.0f, 0.0f, 1.0f,

      0.0f,  size, 0.0f, -1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
      0.0f, 0.0f,  size, -1.0f, 0.0f, 0.0f,
      0.0f,  size,  size, -1.0f, 0.0f, 0.0f,

      size,  size, 0.0f, 1.0f, 0.0f, 0.0f,
      size, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      size, 0.0f,  size, 1.0f, 0.0f, 0.0f,
      size,  size,  size, 1.0f, 0.0f, 0.0f,

      size, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
      size, 0.0f,  size, 0.0f, -1.0f, 0.0f,
      0.0f, 0.0f,  size, 0.0f, -1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,

      size,  size, 0.0f, 0.0f, 1.0f, 0.0f,
      size,  size,  size, 0.0f, 1.0f, 0.0f,
      0.0f,  size,  size, 0.0f, 1.0f, 0.0f,
      0.0f,  size, 0.0f, 0.0f, 1.0f, 0.0f
  };

  this->_indices = std::vector<GLuint>{
      0, 1, 3,
      1, 2, 3,
      4, 5, 7,
      5, 6, 7,
      8, 9, 11,
      9, 10, 11,
      12, 13, 15,
      13, 14, 15,
      16, 17, 19,
      17, 18, 19,
      20, 21, 23,
      21, 22, 23
  };

  this->_modelMatrix = glm::translate(this->_modelMatrix,
      position);
}

CubeGeometry::~CubeGeometry()
{
}

void CubeGeometry::draw(GLFWwindow *window) {
  UNUSED(window);
}

void CubeGeometry::update(double delta) {
  UNUSED(delta);
}

}
