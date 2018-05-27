#include "geometry-node.hpp"

namespace leo {

GeometryNode::GeometryNode() {
  //this->_material = new Material("resources/shaders/vertex-basic.glsl", "resources/shaders/fragment-basic.glsl");
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      this->_modelMatrix[i][j] = i == j ? 1 : 0;
}

GeometryNode::~GeometryNode() {
  /*
  if (this->_material)
    delete this->_material;
    */
}

void GeometryNode::transform(const glm::mat4x4 &transformation) {
  this->_modelMatrix = transformation * this->_modelMatrix;
}

const glm::mat4x4 &GeometryNode::getModelMatrix() const {
  return this->_modelMatrix;
}

}
