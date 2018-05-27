#include "render-visitor.hpp"

namespace leo {

RenderVisitor::RenderVisitor(const Camera *camera, GLFWwindow *window)
  : _camera(camera), _window(window)
{
  this->_material = new Material("resources/shaders/vertex-basic.glsl", "resources/shaders/fragment-basic.glsl");
}

RenderVisitor::~RenderVisitor() {
  delete this->_material;
}

void RenderVisitor::visit(Node *node) {
  this->_visit(node);
}

void RenderVisitor::_visit(Node *node) {
  GeometryNode * g_node = dynamic_cast<GeometryNode*>(node);
  if (g_node) {
    this->_material->use();
    g_node->draw(this->_material);
  }
  for (auto *e : node->getChildren())
    this->_visit(e);
}

}
