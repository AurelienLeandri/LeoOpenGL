#pragma once

#include <model/icomponent.hpp>

#include <utils/geometry.hpp>

#include <vector>

namespace leo
{

class Volume : public IComponent
{
public:
  Volume(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
  Volume(const Volume &other);

protected:
  Volume();

public:
  const std::vector<Vertex> &getVertices() const;
  const std::vector<unsigned int> &getIndices() const;
  void setLabel(std::string label);
  const std::string &getLabel() const;

public:
  static Volume createCustom(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
  static Volume createCube(float side = 1.0);
  static Volume createSphere(float radius = 0.5);
  static Volume createPlane(float width, float height);
  static Volume createPostProcessPlane();

public:
  virtual ComponentType getTypeId() const override
  {
    return ComponentType::VOLUME;
  }

protected:
  void _computeTangents();

protected:
  std::vector<Vertex> _vertices;
  std::vector<unsigned int> _indices;
  std::string _label = "";
};

} // namespace leo
