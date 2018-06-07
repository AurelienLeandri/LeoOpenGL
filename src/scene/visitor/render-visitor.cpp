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
  glm::vec3 camPos = this->_camera->getPosition();
  glUniform3f(glGetUniformLocation(this->_shader->getProgram(), "viewPos"), camPos.x, camPos.y, camPos.z);

  unsigned int ubi = glGetUniformBlockIndex(this->_shader->getProgram(), "Matrices");
  glUniformBlockBinding(this->_shader->getProgram(), ubi, 0);
  unsigned int uboMatrices;
  glGenBuffers(1, &uboMatrices);

  glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMatrices);

  glm::mat4 projection = glm::perspective(this->_camera->getZoom(), (float)800/(float)600, 0.1f, 100.0f);
  glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
  glm::mat4 view = this->_camera->getViewMatrix();
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  this->_lightsUBO.pointLights[0] = this->_pLights[0]->createLightUniform();

  unsigned int ubiLights = glGetUniformBlockIndex(this->_shader->getProgram(), "s1");
  glUniformBlockBinding(this->_shader->getProgram(), ubiLights, 1);
  unsigned int uboLightsData;
  glGenBuffers(1, &uboLightsData);

  glBindBuffer(GL_UNIFORM_BUFFER, uboLightsData);
  glBufferData(GL_UNIFORM_BUFFER, MAX_NUM_LIGHTS * (sizeof(UPointLight) + sizeof(UDirectionLight)),
      NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLightsData);


  glBindBuffer(GL_UNIFORM_BUFFER, uboLightsData);
  int i = 0;
  for (i; i < this->_pLights.size(); i++) {
    this->_lightsUBO.pointLights[i] = this->_pLights[i]->createLightUniform();
    auto &pl = this->_lightsUBO.pointLights[i];
    glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof (UPointLight), sizeof (UPointLight), &pl);
  }
  i = 0;
  unsigned int offset = MAX_NUM_LIGHTS * sizeof (UPointLight);
  for (i; i < this->_dLights.size(); i++) {
    this->_lightsUBO.directionLights[i] = this->_dLights[i]->createLightUniform();
    auto &pl = this->_lightsUBO.directionLights[i];
    glBufferSubData(GL_UNIFORM_BUFFER, offset + i * sizeof (UDirectionLight),
        sizeof (UDirectionLight), &pl);
  }
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

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
    return;
  }
  DirectionLight* dLight = dynamic_cast<DirectionLight*> (light);
  if (dLight) {
    this->_dLights.push_back(dLight);
    return;
  }
}

}
