#pragma once

#include "geometry-node.hpp"

class CubeGeometry : public GeometryNode {
  public:
    CubeGeometry();
    virtual ~CubeGeometry();
    CubeGeometry(double size);
    CubeGeometry(glm::vec3 position, double size);
  public:
    virtual void draw(const OpenGLContext &context);
    virtual void update(double delta);
};
