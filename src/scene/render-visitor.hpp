#pragma once


#include <global.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <camera.hpp>
#include <material/material.hpp>
#include <scene/visitor.hpp>
#include <scene/geometry-node.hpp>

namespace leo {

class RenderVisitor : public Visitor {
  public:
    RenderVisitor(const Camera *_camera, GLFWwindow *window);
    virtual ~RenderVisitor();

  public:
    virtual void visit(Node *node);

  private:
    virtual void _visit(Node *node);

  private:
    const Camera *_camera;
    GLFWwindow *_window;
    Material *_material;
};

}
