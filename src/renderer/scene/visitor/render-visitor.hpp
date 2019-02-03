#pragma once


#include <renderer/global.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <renderer/camera.hpp>
#include <utils/shader.hpp>
#include <utils/framebuffer.hpp>
#include <renderer/scene/visitor/visitor.hpp>
#include <renderer/scene/geometry-node.hpp>
#include <renderer/scene/cube-map.hpp>
#include <renderer/scene/alpha-node.hpp>
#include <renderer/scene/lights/point-light.hpp>
#include <renderer/scene/lights/direction-light.hpp>

namespace leo {

  #define MAX_NUM_LIGHTS 10

  typedef struct uboLights {
    PointLightUniform pointLights[MAX_NUM_LIGHTS];
    DirectionLightUniform directionLights[MAX_NUM_LIGHTS];
  } uboLights;

class RenderVisitor : public Visitor {
  public:
    RenderVisitor(const Camera *_camera, GLFWwindow *window);
    RenderVisitor(const Camera *_camera, GLFWwindow *window,
        const GLchar *vertex, const GLchar *fragment);
    RenderVisitor(const Camera *_camera, GLFWwindow *window, std::shared_ptr<Shader> shader);
    virtual ~RenderVisitor();

  public:
    void _init();
    virtual void visit(Node *node);
    virtual void visit(Node *node, bool offscreen);
    virtual void visit(Node *node, bool offscreen, bool clear);

  public:
    void registerLight(Light *light);
    void registerFramebuffer(const Framebuffer &fb);

  private:
    virtual void _visit(Node *node);
    void _setupRendering(bool offscreen, bool clear);

  public:
    const Framebuffer &getFramebuffer() const { return this->_fb; }
    const std::shared_ptr<Shader> getShader() const { return this->_shader; }
    void setFramebuffer(const Framebuffer &value) { this->_fb = value; }
    void setCubeMapTexture(const Texture *texture) {
      this->_cubeMapTexture = texture;
      this->_shader->setTextureOffset(this->_colorBuffers.size() + (this->_cubeMapTexture ? 1 : 0));
    }

  private:
    const Camera *_camera;
    GLFWwindow *_window;
    std::shared_ptr<Shader> _shader;
    std::vector<PointLight *> _pLights;
    std::vector<DirectionLight *> _dLights;
    Framebuffer _fb;
    std::vector<const Texture *> _colorBuffers;
    uboLights _lightsData;
    GLuint _lightsUBO;
    const Texture *_cubeMapTexture = nullptr;

};

}
