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
  glm::vec3 camPos = this->_camera->getPosition();
  glUniform3f(glGetUniformLocation(this->_material->getProgram(), "viewPos"), camPos.x, camPos.y, camPos.z);

  for (int i = 1; i <= this->_pLights.size(); i++) {
    // TODO: Put all that in UBO and refactor
    UPointLight lightData = this->_pLights[i - 1]->createLightUniform();
    glUniform1f(glGetUniformLocation(this->_material->getProgram(), "pLight1.constant"), lightData.constant);
    glUniform1f(glGetUniformLocation(this->_material->getProgram(), "pLight1.linear"), lightData.linear);
    glUniform1f(glGetUniformLocation(this->_material->getProgram(), "pLight1.quadratic"), lightData.quadratic);
    glUniform3f(glGetUniformLocation(this->_material->getProgram(), "pLight1.ambient"),
        lightData.ambient.x, lightData.ambient.y, lightData.ambient.z);
    glUniform3f(glGetUniformLocation(this->_material->getProgram(), "pLight1.diffuse"),
        lightData.diffuse.x, lightData.diffuse.y, lightData.diffuse.z);
    glUniform3f(glGetUniformLocation(this->_material->getProgram(), "pLight1.specular"),
        lightData.specular.x, lightData.specular.y, lightData.specular.z);
    glUniform3f(glGetUniformLocation(this->_material->getProgram(), "pLight1.position"),
        lightData.position.x, lightData.position.y, lightData.position.z);
  }

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

void RenderVisitor::registerLight(Light *light) {
  PointLight* pLight = dynamic_cast<PointLight*> (light);
  if (pLight) {
    this->_pLights.push_back(pLight);
  }

}

}
