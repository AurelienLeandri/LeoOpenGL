#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <SFML/Window.hpp>

#include <camera.hpp>
#include <material/material.hpp>
#include <scene/visitor.hpp>
#include <scene/geometry-node.hpp>

namespace leo {

class RenderVisitor : public Visitor {
  public:
    RenderVisitor(const Camera *_camera, sf::Window *window);

  public:
    virtual void visit(Node *node);

  private:
    virtual void _visit(Node *node);

  private:
    const Camera *_camera;
    sf::Window *_window;
};

}
