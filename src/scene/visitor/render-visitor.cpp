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
    this->_init();
    this->_shader= std::make_shared<Shader>(Shader(vertex, fragment));
  }

  RenderVisitor::RenderVisitor(const Camera *camera, GLFWwindow *window, std::shared_ptr<Shader> shader)
    : _camera(camera), _window(window)
  {
    this->_init();
    this->_shader = shader;
  }

  RenderVisitor::~RenderVisitor() {
  }

  void RenderVisitor::_init() {
      glGenBuffers(1, &this->_lightsUBO);

      glBindBuffer(GL_UNIFORM_BUFFER, this->_lightsUBO);
      glBufferData(GL_UNIFORM_BUFFER,
          MAX_NUM_LIGHTS * (sizeof(PointLightUniform) + sizeof(DirectionLightUniform)),
          NULL, GL_DYNAMIC_DRAW
          );
      glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  void RenderVisitor::visit(Node *node) {
    this->visit(node, false);
  }

  void RenderVisitor::visit(Node *node, bool offscreen) {
    this->_setupRendering(offscreen, true);
    this->_visit(node);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void RenderVisitor::_visit(Node *node) {
    // TODO: cleaner, as a cube node is also a geometry node!!
    CubeMap* cubeNode = dynamic_cast<CubeMap*>(node);
    if (cubeNode)
    {
      unsigned int ubi = glGetUniformBlockIndex(this->_shader->getProgram(), "Matrices");
      glUniformBlockBinding(this->_shader->getProgram(), ubi, 0);
      unsigned int uboMatrices;
      glGenBuffers(1, &uboMatrices);

      glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
      glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
      glBindBuffer(GL_UNIFORM_BUFFER, 0);

      glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMatrices);

      glm::mat4 projection = glm::perspective(this->_camera->getZoom(), (float)800/(float)600, 0.1f, 100.0f);
      glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
      glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
      glm::mat4 view = glm::mat4(glm::mat3(this->_camera->getViewMatrix()));
      glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
      glBindBuffer(GL_UNIFORM_BUFFER, 0);


      glDepthMask(GL_FALSE);
      cubeNode->draw(this->_shader.get());
      glDepthMask(GL_TRUE);
      return;
    }
    GeometryNode* geometryNode = dynamic_cast<GeometryNode*>(node);
    if (geometryNode)
    {
      // Draw the loaded model
      const glm::mat4 &model = geometryNode->getModelMatrix();
      glUniformMatrix4fv(glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
      geometryNode->draw(this->_shader.get());
      for (auto *e : node->getChildren())
        this->_visit(e);
      return;
    }
    AlphaNode *alphaNode = dynamic_cast<AlphaNode*>(node);
    if (alphaNode) {
      for (auto &e : alphaNode->getSortedChildren())
        this->_visit(e.second);
      for (auto *e : node->getChildren())
        this->_visit(e);
      return;
    }
  }

  void RenderVisitor::_setupRendering(bool offscreen, bool clear) {
    if (offscreen) {
      glBindFramebuffer(GL_FRAMEBUFFER, this->_fb.getId());
      glEnable(GL_DEPTH_TEST);
      if (clear)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glDisable(GL_DEPTH_TEST);
      if (clear)
        glClear(GL_COLOR_BUFFER_BIT);
    }

    this->_shader->use();
    if (offscreen) {
      // Transformation matrices
      glm::vec3 camPos = this->_camera->getPosition();
      glUniform3f(glGetUniformLocation(this->_shader->getProgram(), "viewPos"), camPos.x, camPos.y, camPos.z);

      unsigned int ubi = glGetUniformBlockIndex(this->_shader->getProgram(), "Matrices");
      glUniformBlockBinding(this->_shader->getProgram(), ubi, 0);
      unsigned int uboMatrices;
      glGenBuffers(1, &uboMatrices);

      glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
      glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
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

      glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->_lightsUBO);

    }

    for (GLuint i = 0; i < this->_colorBuffers.size(); i++) {
      std::stringstream number;
      number << i;
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, this->_colorBuffers[i]->id);
      glUniform1i(glGetUniformLocation(this->_shader->getProgram(), ("fb" + number.str()).c_str()), i);
    }
    glActiveTexture(GL_TEXTURE0);
  }  // void RenderVisitor::_setupRendering(bool offscreen)

  void RenderVisitor::registerLight(Light *light) {
    PointLight* pLight = dynamic_cast<PointLight*> (light);
    DirectionLight* dLight = dynamic_cast<DirectionLight*> (light);
    if (pLight)
      this->_pLights.push_back(pLight);
    else if (dLight)
        this->_dLights.push_back(dLight);
    else
      return;

    glBindBuffer(GL_UNIFORM_BUFFER, this->_lightsUBO);
    unsigned int i = 0;
    for (; i < this->_pLights.size(); i++) {
      this->_lightsData.pointLights[i] = PointLightUniform(*this->_pLights[i]);
      auto &pl = this->_lightsData.pointLights[i];
      glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof (PointLightUniform), sizeof (PointLightUniform), &pl);
    }
    i = 0;
    unsigned int offset = MAX_NUM_LIGHTS * sizeof (PointLightUniform);
    for (; i < this->_dLights.size(); i++) {
      this->_lightsData.directionLights[i] = DirectionLightUniform(*this->_dLights[i]);
      auto &pl = this->_lightsData.directionLights[i];
      glBufferSubData(GL_UNIFORM_BUFFER, offset + i * sizeof (DirectionLightUniform),
          sizeof (DirectionLightUniform), &pl);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

  }

  void RenderVisitor::registerFramebuffer(const Framebuffer &fb) {
    auto &color_buffers = fb.getColorBuffers();
    for (unsigned int i = 0; i < color_buffers.size(); i++)
      this->_colorBuffers.push_back(&color_buffers[i]);
    this->_shader->setTextureOffset(this->_colorBuffers.size());
  }


}
