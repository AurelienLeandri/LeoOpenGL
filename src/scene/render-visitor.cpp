#include "render-visitor.hpp"
#include "geometry-node.hpp"
#include "../material/material.hpp"

RenderVisitor::RenderVisitor(const Camera *camera, sf::Window *window)
  : _camera(camera), _window(window)
{
}

void RenderVisitor::visit(Node *node) {
  this->_visit(node);
  this->_window->display();  // TODO: move display in a calling function after the recursive visit
}

void RenderVisitor::_visit(Node *node) {
  GeometryNode * g_node = dynamic_cast<GeometryNode*>(node);
  if (g_node) {
    const float *data = g_node->getVBO();
    const float *indices = g_node->getVBO();
    const Material *material = g_node->getMaterial;
    material->use();
    glm::mat4 view = this->_camera->getViewMatrix();

    // TODO: change 800*600 for real screen size
    glm::mat4 projection = glm::perspective(this->_camera->getZoom(), (float)800/(float)600, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(material->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(material->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Cubes TODO: bind VBO instead of VAO
    glBindVertexArray(cubeVAO); {

      // TODO: manage textures
      //glBindTexture(GL_TEXTURE_2D, cubeTexture);  // We omit the glActiveTexture part since TEXTURE0 is already the default active texture unit. (sampler used in fragment is set to 0 as well as default)

      glUniformMatrix4fv(glGetUniformLocation(material->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(g_node->getModelMatrix()));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    } glBindVertexArray(0);
  }
  for (auto *e : node->getChildren())
    this->_visit(e);
}
