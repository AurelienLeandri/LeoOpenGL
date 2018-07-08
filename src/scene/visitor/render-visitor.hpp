#pragma once


#include <global.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <camera.hpp>
#include <utils/shader.hpp>
#include <scene/visitor/visitor.hpp>
#include <scene/geometry-node.hpp>
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
    RenderVisitor(const Camera *_camera, GLFWwindow *window, bool offscreen);
    RenderVisitor(const Camera *_camera, GLFWwindow *window,
        const GLchar *vertex, const GLchar *fragment, bool offscreen);
    virtual ~RenderVisitor();

  public:
    void _init();
    virtual void visit(Node *node);

  public:
    void registerLight(Light *light);
    void registerFrameBuffer(const RenderVisitor &rv);

  private:
    virtual void _visit(Node *node);

  public:
    const GLuint &getColorBufferTexture() const { return this->_colorBufferTexture; }

  private:
    const Camera *_camera;
    GLFWwindow *_window;
    Shader *_shader;
    std::vector<PointLight *> _pLights;
    std::vector<DirectionLight *> _dLights;
    uboLights _lightsUBO;
    GLuint _fbo;
    GLuint _rbo;
    GLuint _colorBufferTexture;
    std::vector<GLuint> _fbTextures;
    bool _offscreen;
};

}
