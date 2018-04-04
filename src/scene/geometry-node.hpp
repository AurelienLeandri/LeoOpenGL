#pragma once

#include <glm/glm.hpp>
#include <SFML/Window.hpp>

#include <scene/node.hpp>
#include <material/material.hpp>

namespace leo {

class GeometryNode : public Node {
  public:
    GeometryNode();
    virtual ~GeometryNode();
    virtual void draw(sf::Window *window) = 0;
    virtual void update(double delta) = 0;

  public:
    void transform(const glm::mat4x4 &transformation);

  public:
    void setMaterial(Material *material);
    const glm::mat4x4 &getModelMatrix() const;
    const Material *getMaterial() const;
    const float *getVBO() const;
    const float *getEBO() const;
    size_t getDataSize() const;
    size_t getIndicesSize() const;

  protected:
    Material *_material = nullptr;
    std::vector<float> _data;
    std::vector<float> _indices;
    glm::mat4x4 _modelMatrix;
};

}
