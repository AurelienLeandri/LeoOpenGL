#pragma once

#include <model/icomponent.hpp>
#include <model/texture-manager.hpp>

#include <renderer/global.hpp>

namespace leo
{

class Texture;

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
  Texture *diffuse_texture = TextureManager::white.get();
  glm::vec3 specular_value;
  Texture *specular_texture = TextureManager::white.get();
  Texture *reflection_map = TextureManager::black.get();
  Texture *normal_map = TextureManager::blue.get();
  Texture *parallax_map = TextureManager::black.get();
  float shininess = 32.f;
  glm::vec3 emissive_value;
  bool force = false;
};

} // namespace leo
