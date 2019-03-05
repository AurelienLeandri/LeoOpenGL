#pragma once

#include <model/icomponent.hpp>

#include <utils/geometry.hpp>

#include <vector>

namespace leo
{
namespace model
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

public:
  static Volume createCustom(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
  static Volume createCube(float side);
  static Volume createPlane(float width, float height);

public:
  virtual ComponentType getTypeId() const override
  {
    return ComponentType::VOLUME;
  }

protected:
  std::vector<Vertex> _vertices;
  std::vector<unsigned int> _indices;
};

} // namespace model
} // namespace leo
