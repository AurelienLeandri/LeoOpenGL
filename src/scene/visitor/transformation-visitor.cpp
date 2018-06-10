#include "transformation-visitor.hpp"

namespace leo {

TransformationVisitor::TransformationVisitor() {
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      this->_transformation[i][j] = i == j ? 1 : 0;
}

TransformationVisitor::TransformationVisitor(const TransformationVisitor &other) :
  Visitor(other),
  _transformation(other._transformation)
{
}


TransformationVisitor::~TransformationVisitor() {
}

TransformationVisitor &TransformationVisitor::operator=(const TransformationVisitor &other) {
  Visitor::operator=(other);
  this->_transformation = other._transformation;
  return *this;
}

void TransformationVisitor::visit(Node *node) {
  GeometryNode * g_node = dynamic_cast<GeometryNode*>(node);
  if (g_node)
    g_node->transform(this->_transformation);
  for (auto *e : node->getChildren())
    this->visit(e);
}

void TransformationVisitor::translate(glm::vec3 v) {
  this->_transformation = glm::translate(this->_transformation, v);
}

void TransformationVisitor::rotate(float deg, glm::vec3 v) {
  this->_transformation = glm::rotate(this->_transformation, deg, v);
}

void TransformationVisitor::scale(glm::vec3 v) {
  this->_transformation = glm::scale(this->_transformation, v);
}

void TransformationVisitor::transform(glm::mat4x4 transformation) {
  this->_transformation = transformation * this->_transformation;
}

void TransformationVisitor::setTransformation(glm::mat4x4 transformation){
  this->_transformation = transformation;
}

}
