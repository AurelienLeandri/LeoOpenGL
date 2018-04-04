#pragma once

#include <scene/geometry-node.hpp>

namespace leo {

class CubeGeometry : public GeometryNode {
  public:
    CubeGeometry();
    virtual ~CubeGeometry();
    CubeGeometry(double size);
    CubeGeometry(glm::vec3 position, double size);
  public:
    virtual void draw(sf::Window *window);
    virtual void update(double delta);
};

}
