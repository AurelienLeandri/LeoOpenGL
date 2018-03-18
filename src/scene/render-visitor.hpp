#pragma once

#include <SFML/Window.hpp>

#include "visitor.hpp"
#include "../camera.hpp"

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
