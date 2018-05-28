#pragma once

#include <global.hpp>

#include <scene/node.hpp>
#include <utils/shader.hpp>

namespace leo {

class GeometryNode : public Node {
  public:
    GeometryNode();
    virtual ~GeometryNode();
    virtual void draw(Shader *shader) = 0;
    virtual void update(double delta) = 0;
    virtual void addChild(Node *child) override;

  public:
    void transform(const glm::mat4x4 &transformation);

  public:
    const glm::mat4x4 &getModelMatrix() const;

  protected:
    glm::mat4x4 _modelMatrix;
};

}
