#include "render-visitor.hpp"

namespace leo {

RenderVisitor::RenderVisitor(const Camera *camera, GLFWwindow *window, bool offscreen)
  : RenderVisitor(camera, window,
      "resources/shaders/vertex-basic.glsl",
      "resources/shaders/fragment-basic.glsl", offscreen)
{
}

RenderVisitor::RenderVisitor(const Camera *camera, GLFWwindow *window,
    const GLchar *vertex, const GLchar *fragment, bool offscreen)
  : _camera(camera), _window(window), _offscreen(offscreen)
{
  this->_init();
  this->_shader= new Shader(vertex, fragment);
}

RenderVisitor::~RenderVisitor() {
  if (this->_offscreen)
    glDeleteFramebuffers(1, &this->_fbo);
  delete this->_shader;
}

void RenderVisitor::_init() {
  if (this->_offscreen) {
    // FrameBuffer initialization
    glGenFramebuffers(1, &this->_fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);

    // generate texture
    glGenTextures(1, &this->_colorBufferTexture);
    glBindTexture(GL_TEXTURE_2D, this->_colorBufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->_colorBufferTexture, 0);

    glGenRenderbuffers(1, &this->_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, this->_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->_rbo);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

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

  unsigned int ubiLights = glGetUniformBlockIndex(this->_shader->getProgram(), "s1");
  glUniformBlockBinding(this->_shader->getProgram(), ubiLights, 1);
  unsigned int uboLightsData;
  glGenBuffers(1, &uboLightsData);

  glBindBuffer(GL_UNIFORM_BUFFER, uboLightsData);
  glBufferData(GL_UNIFORM_BUFFER, MAX_NUM_LIGHTS * (sizeof(PointLightUniform) + sizeof(DirectionLightUniform)),
      NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLightsData);


  glBindBuffer(GL_UNIFORM_BUFFER, uboLightsData);
  unsigned int i = 0;
  for (; i < this->_pLights.size(); i++) {
    this->_lightsUBO.pointLights[i] = PointLightUniform(*this->_pLights[i]);
    auto &pl = this->_lightsUBO.pointLights[i];
    glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof (PointLightUniform), sizeof (PointLightUniform), &pl);
  }
  i = 0;
  unsigned int offset = MAX_NUM_LIGHTS * sizeof (PointLightUniform);
  for (; i < this->_dLights.size(); i++) {
    this->_lightsUBO.directionLights[i] = DirectionLightUniform(*this->_dLights[i]);
    auto &pl = this->_lightsUBO.directionLights[i];
    glBufferSubData(GL_UNIFORM_BUFFER, offset + i * sizeof (DirectionLightUniform),
        sizeof (DirectionLightUniform), &pl);
  }
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  if (this->_offscreen)
    glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);

  // Bind Framebuffer textures
  for (size_t i = 0; i < this->_fbTextures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, this->_fbTextures[i]);
  }

  this->_visit(node);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
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


void RenderVisitor::registerFrameBuffer(const RenderVisitor &rv) {
  GLuint cbt = rv.getColorBufferTexture();
  if (cbt > 0) {
    this->_fbTextures.push_back(cbt);
  }
}

}
