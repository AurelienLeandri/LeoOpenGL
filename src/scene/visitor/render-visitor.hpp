#pragma once


#include <global.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <camera.hpp>
#include <utils/shader.hpp>
#include <utils/framebuffer.hpp>
#include <scene/visitor/visitor.hpp>
#include <scene/geometry-node.hpp>
#include <scene/cube-map.hpp>
#include <scene/alpha-node.hpp>
#include <scene/lights/point-light.hpp>
#include <scene/lights/direction-light.hpp>

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
};

}
