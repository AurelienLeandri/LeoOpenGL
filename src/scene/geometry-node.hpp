#pragma once

#include <global.hpp>

#include <scene/drawable-node.hpp>
#include <utils/shader.hpp>

namespace leo {

class GeometryNode : public DrawableNode {
  public:
    GeometryNode();
    virtual ~GeometryNode();
    GeometryNode(const GeometryNode &other);
    GeometryNode &operator=(const GeometryNode &other);

  public:
    virtual void draw(Shader *shader) = 0;
    virtual void update(double delta) = 0;

  public:
    virtual void transform(const glm::mat4x4 &transformation);

  public:
    const glm::mat4x4 &getModelMatrix() const;
    glm::vec3 getPosition() const;

  protected:
    glm::mat4x4 _modelMatrix;
};

}
