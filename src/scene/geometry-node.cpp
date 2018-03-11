#include "geometry-node.hpp"

GeometryNode::~GeometryNode() {
}

void GeometryNode::setMaterial(Material *material) {
  this->_material = material;
}
