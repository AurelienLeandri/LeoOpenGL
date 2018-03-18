#pragma once

#include "node.hpp"
#include "../material/material.hpp"
#include "../opengl-context.hpp"

class GeometryNode : public Node {
  public:
    GeometryNode();
    virtual ~GeometryNode();
    virtual void draw(const OpenGLContext &context) = 0;

  public:
    const glm::mat4x4 &getModelMatrix() const;
    void transform(const glm::mat4x4 &transformation);

  public:
    void setMaterial(Material *material);
    const glm::mat4x4 &getModelMatrix();
    const Material *getMaterial;
    const float *getVBO();
    const float *getEBO();

  protected:
    Material *_material = nullptr;
    std::vector<float> _data;
    std::vector<float> _indices;
    glm::mat4x4 _modelMatrix;
};
