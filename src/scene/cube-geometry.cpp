#include "cube-geometry.hpp"

namespace leo {

CubeGeometry::CubeGeometry() : CubeGeometry(glm::vec3(0, 0, 0), 1.0) {
}

CubeGeometry::CubeGeometry(double size)
  : CubeGeometry(glm::vec3(0, 0, 0), size)
{
}

CubeGeometry::CubeGeometry(glm::vec3 position, double size) {
  // FIXME: translate model matrix
}

CubeGeometry::~CubeGeometry()
{
}

void CubeGeometry::draw(sf::Window *window) {
}

void CubeGeometry::update(double delta) {
}

}
