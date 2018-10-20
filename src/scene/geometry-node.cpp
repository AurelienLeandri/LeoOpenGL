#include "geometry-node.hpp"

namespace leo {

GeometryNode::GeometryNode() {
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      this->_modelMatrix[i][j] = i == j ? 1 : 0;
}

GeometryNode::~GeometryNode() {
}

GeometryNode::GeometryNode(const GeometryNode &other) :
  _modelMatrix(other._modelMatrix)
{
}

GeometryNode &GeometryNode::operator=(const GeometryNode &other) {
  this->_modelMatrix = other._modelMatrix;
  return *this;
}

void GeometryNode::transform(const glm::mat4x4 &transformation) {
  this->_modelMatrix = transformation * this->_modelMatrix;
}

const glm::mat4x4 &GeometryNode::getModelMatrix() const {
  return this->_modelMatrix;
}

glm::vec3 GeometryNode::getPosition() const {
  return glm::vec3(_modelMatrix[3][0], _modelMatrix[3][1], _modelMatrix[3][2]);
}

}
