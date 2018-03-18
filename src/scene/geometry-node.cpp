#include "geometry-node.hpp"

GeometryNode::~GeometryNode() {
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      this->_modelMatrix[i][j] = i == j ? 1 : 0;
}

void GeometryNode::transform(const glm::mat4x4 &transformation) {
  this->_modelMatrix = transformation * this->_modelMatrix;
}

const glm::mat4x4 &GeometryNode::getModelMatrix() {
  return this->_modelMatrix;
}

void GeometryNode::setMaterial(Material *material) {
  this->_material = material;
}
