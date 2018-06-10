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
    UPointLight pointLights[MAX_NUM_LIGHTS];
    UDirectionLight directionLights[MAX_NUM_LIGHTS];
  } uboLights;

class RenderVisitor : public Visitor {
  public:
    RenderVisitor(const Camera *_camera, GLFWwindow *window);
    RenderVisitor(const Camera *_camera, GLFWwindow *window,
        const GLchar *vertex, const GLchar *fragment);
    virtual ~RenderVisitor();
    RenderVisitor(const RenderVisitor &other);
    RenderVisitor &operator=(const RenderVisitor &other);

  public:
    virtual void visit(Node *node);

  public:
    void registerLight(Light *light);

  private:
    virtual void _visit(Node *node);

  private:
    const Camera *_camera;
    GLFWwindow *_window;
    Shader *_shader;
    std::vector<PointLight *> _pLights;
    std::vector<DirectionLight *> _dLights;
    uboLights _lightsUBO;
};

}