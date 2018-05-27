#include "render-visitor.hpp"

namespace leo {

RenderVisitor::RenderVisitor(const Camera *camera, GLFWwindow *window)
  : RenderVisitor(camera, window,
      "resources/shaders/vertex-basic.glsl",
      "resources/shaders/fragment-basic.glsl")
{
}

RenderVisitor::RenderVisitor(const Camera *camera, GLFWwindow *window,
    const GLchar *vertex, const GLchar *fragment)
  : _camera(camera), _window(window)
{
  this->_material = new Material(vertex, fragment);
}

RenderVisitor::~RenderVisitor() {
  delete this->_material;
}

void RenderVisitor::visit(Node *node) {
  this->_material->use();

  // Transformation matrices
  glm::mat4 projection = glm::perspective(this->_camera->getZoom(), (float)800/(float)600, 0.1f, 100.0f);
  glm::mat4 view = this->_camera->getViewMatrix();
  glUniformMatrix4fv(glGetUniformLocation(this->_material->getProgram(), "projection"), 1, GL_FALSE,
      glm::value_ptr(projection));
  glUniformMatrix4fv(glGetUniformLocation(this->_material->getProgram(), "view"), 1, GL_FALSE,
      glm::value_ptr(view));

  this->_visit(node);
}

void RenderVisitor::_visit(Node *node) {
  GeometryNode * g_node = dynamic_cast<GeometryNode*>(node);
  if (g_node) {
    // Draw the loaded model
    const glm::mat4 &model = g_node->getModelMatrix();
    glUniformMatrix4fv(glGetUniformLocation(this->_material->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    g_node->draw(this->_material);
  }
  for (auto *e : node->getChildren())
    this->_visit(e);
}

}
