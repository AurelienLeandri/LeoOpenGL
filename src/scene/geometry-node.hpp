#pragma once

#include <global.hpp>

#include <scene/node.hpp>
#include <utils/shader.hpp>

namespace leo {

  enum GeometryNodeOptions {
    TRANSPARENT = 1 << 0
  };

class GeometryNode : public Node {
  public:
    GeometryNode();
    virtual ~GeometryNode();
    GeometryNode(const GeometryNode &other);
    GeometryNode &operator=(const GeometryNode &other);

  public:
    virtual void draw(Shader *shader) = 0;
    virtual void update(double delta) = 0;
    virtual void addChild(Node *child) override;

  public:
    virtual void transform(const glm::mat4x4 &transformation);

  public:
    const glm::mat4x4 &getModelMatrix() const;
    glm::vec3 getPosition() const;
    void setGeometryNodeOptions(unsigned int mask) { this->_geometryNodeOptions = mask; }
    unsigned int getGeometryNodeOptions() const { return this->_geometryNodeOptions; }

  protected:
    glm::mat4x4 _modelMatrix;
    unsigned int _geometryNodeOptions = 0;
};

}
