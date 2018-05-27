#pragma once

#include <scene/geometry-node.hpp>
#include <material/material.hpp>

namespace leo {

class CubeGeometry : public GeometryNode {
  public:
    CubeGeometry();
    virtual ~CubeGeometry();
    CubeGeometry(float size);
    CubeGeometry(glm::vec3 position, float size);
  public:
    //virtual void draw(GLFWwindow *window);
    virtual void draw(Material *material);
    virtual void update(double delta);
};

}
