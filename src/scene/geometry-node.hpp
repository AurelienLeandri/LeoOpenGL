#pragma once

#include <global.hpp>

#include <scene/node.hpp>
#include <material/material.hpp>

namespace leo {

class GeometryNode : public Node {
  public:
    GeometryNode();
    virtual ~GeometryNode();
    virtual void draw(GLFWwindow *window) = 0;
    virtual void update(double delta) = 0;

  public:
    void transform(const glm::mat4x4 &transformation);

  public:
    void setMaterial(Material *material);
    const glm::mat4x4 &getModelMatrix() const;
    const Material *getMaterial() const;
    const GLfloat *getVBO() const;
    const GLuint *getEBO() const;
    size_t getDataSize() const;
    size_t getIndicesSize() const;

  protected:
    Material *_material = nullptr;
    std::vector<GLfloat> _data;
    std::vector<GLuint> _indices;
    glm::mat4x4 _modelMatrix;
};

}
