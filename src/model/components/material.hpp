#pragma once

#include <model/icomponent.hpp>

#include <renderer/global.hpp>

namespace leo
{

class Texture;

namespace model
{

class Material : public IComponent
{
public:
  Material();
  Material(bool force);

public:
  virtual ComponentType getTypeId() const override
  {
    return ComponentType::MATERIAL;
  }

public:
  glm::vec3 diffuse_value;
  Texture *diffuse_texture = nullptr;
  glm::vec3 specular_value;
  Texture *specular_texture = nullptr;
  Texture *reflection_map = nullptr;
  float shininess = 32.f;
  bool force = false;

};

} // namespace model
} // namespace leo
