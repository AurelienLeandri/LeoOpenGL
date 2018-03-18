#include "geometry-node.hpp"

namespace leo {

GeometryNode::GeometryNode() {
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      this->_modelMatrix[i][j] = i == j ? 1 : 0;
}

GeometryNode::~GeometryNode() {
  if (this->_material)
    delete this->_material;
}

void GeometryNode::transform(const glm::mat4x4 &transformation) {
  this->_modelMatrix = transformation * this->_modelMatrix;
}

void GeometryNode::setMaterial(Material *material) {
  this->_material = material;
}


const glm::mat4x4 &GeometryNode::getModelMatrix() const {
  return this->_modelMatrix;
}

const Material *GeometryNode::getMaterial() const {
  return this->_material;
}

const float *GeometryNode::getVBO() const {
  return &_data[0];
}

const float *GeometryNode::getEBO() const {
  return &_indices[0];
}

size_t GeometryNode::getDataSize() const {
  return this->_data.size();
}

size_t GeometryNode::getIndicesSize() const {
  return this->_indices.size();
}

}
