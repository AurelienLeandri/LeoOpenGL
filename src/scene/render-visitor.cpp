#include "render-visitor.hpp"

namespace leo {

RenderVisitor::RenderVisitor(const Camera *camera, sf::Window *window)
  : _camera(camera), _window(window)
{
}

void RenderVisitor::visit(Node *node) {
  this->_visit(node);
  //this->_window->display();  // TODO: move display in a calling function after the recursive visit
}

void RenderVisitor::_visit(Node *node) {
  GeometryNode * g_node = dynamic_cast<GeometryNode*>(node);
  if (g_node) {
    // TODO: if geometry hasnt changed, maybe put genBuffers in the node info (check if no limited size)
    const float *data = g_node->getVBO();
    const float *indices = g_node->getVBO();
    const Material *material = g_node->getMaterial();
    material->use();
    //glm::mat4 view = this->_camera->getViewMatrix();
    glm::mat4 view;

    GLuint EBO;
    glGenBuffers(1, &EBO);
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    GLuint VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO); {
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, g_node->getDataSize(), data, GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_node->getIndicesSize(), indices,
          GL_STATIC_DRAW);

      // TODO: variable buffer element size and offset ?
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
          (GLvoid *)0);
      glEnableVertexAttribArray(0);

      // TODO: variable buffer element size and offset ?
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
          (GLvoid *)(3 * sizeof(GLfloat)));
      glEnableVertexAttribArray(1);

    } glBindVertexArray(0);

    // TODO: change 800*600 for real screen size
    glm::mat4 projection;
    /*
    glm::mat4 projection = glm::perspective(this->_camera->getZoom(),
        (float) 800 / (float) 600, 0.1f, 100.0f);
        */

    glUniformMatrix4fv(glGetUniformLocation(material->getProgram(), "view"),
        1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(material->getProgram(), "projection"),
        1, GL_FALSE, glm::value_ptr(projection));

    // Cubes TODO: bind VBO instead of VAO
    glBindVertexArray(VAO); {
      // TODO: manage textures
      //glBindTexture(GL_TEXTURE_2D, cubeTexture);  // We omit the glActiveTexture part since TEXTURE0 is already the default active texture unit. (sampler used in fragment is set to 0 as well as default)

      glUniformMatrix4fv(glGetUniformLocation(material->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(g_node->getModelMatrix()));
      glDrawElements(GL_TRIANGLES, g_node->getIndicesSize() / 3, GL_UNSIGNED_INT, 0);
    } glBindVertexArray(0);
  }
  for (auto *e : node->getChildren())
    this->_visit(e);
}

}
