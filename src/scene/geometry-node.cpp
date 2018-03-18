??? from here until ???END lines may have been inserted/deleted

#include "geometry-node.hpp"

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


const glm::mat4x4 &GeometryNode::getModelMatrix() {
  return this->_modelMatrix;
}

const Material *GeometryNode::getMaterial() {
  return this->_material;
}

const float *GeometryNode::getVBO() {
  return &_data[0];
}

const float *GeometryNode::getEBO() {
  return &_indices[0];
}
???END
