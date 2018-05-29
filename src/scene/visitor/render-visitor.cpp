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
  this->_shader= new Shader(vertex, fragment);
}

RenderVisitor::~RenderVisitor() {
  delete this->_shader;
}

void RenderVisitor::visit(Node *node) {
  this->_shader->use();

  // Transformation matrices
  glm::mat4 projection = glm::perspective(this->_camera->getZoom(), (float)800/(float)600, 0.1f, 100.0f);
  glm::mat4 view = this->_camera->getViewMatrix();
  glUniformMatrix4fv(glGetUniformLocation(this->_shader->getProgram(), "projection"), 1, GL_FALSE,
      glm::value_ptr(projection));
  glUniformMatrix4fv(glGetUniformLocation(this->_shader->getProgram(), "view"), 1, GL_FALSE,
      glm::value_ptr(view));
  glm::vec3 camPos = this->_camera->getPosition();
  glUniform3f(glGetUniformLocation(this->_shader->getProgram(), "viewPos"), camPos.x, camPos.y, camPos.z);

  for (int i = 1; i <= this->_pLights.size(); i++) {
    // TODO: Put all that in UBO and refactor
    UPointLight lightData = this->_pLights[i - 1]->createLightUniform();
    glUniform1f(glGetUniformLocation(this->_shader->getProgram(),
          Shader::generateParamName("pLight", i, ".constant").c_str()), lightData.constant);
    glUniform1f(glGetUniformLocation(this->_shader->getProgram(),
          Shader::generateParamName("pLight", i, ".linear").c_str()), lightData.linear);
    glUniform1f(glGetUniformLocation(this->_shader->getProgram(),
        Shader::generateParamName("pLight", i, ".quadratic").c_str()), lightData.quadratic);
    glUniform3f(glGetUniformLocation(this->_shader->getProgram(),
        Shader::generateParamName("pLight", i, ".ambient").c_str()),
        lightData.ambient.x, lightData.ambient.y, lightData.ambient.z);
    glUniform3f(glGetUniformLocation(this->_shader->getProgram(),
        Shader::generateParamName("pLight", i, ".diffuse").c_str()),
        lightData.diffuse.x, lightData.diffuse.y, lightData.diffuse.z);
    glUniform3f(glGetUniformLocation(this->_shader->getProgram(),
        Shader::generateParamName("pLight", i, ".specular").c_str()),
        lightData.specular.x, lightData.specular.y, lightData.specular.z);
    glUniform3f(glGetUniformLocation(this->_shader->getProgram(),
        Shader::generateParamName("pLight", i, ".position").c_str()),
        lightData.position.x, lightData.position.y, lightData.position.z);
  }

  this->_visit(node);
}

void RenderVisitor::_visit(Node *node) {
  GeometryNode * g_node = dynamic_cast<GeometryNode*>(node);
  if (g_node) {
    // Draw the loaded model
    const glm::mat4 &model = g_node->getModelMatrix();
    glUniformMatrix4fv(glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    g_node->draw(this->_shader);
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
