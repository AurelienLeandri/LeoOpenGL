#pragma once

#include <model/icomponent.hpp>
#include <model/texture-manager.hpp>

#include <renderer/global.hpp>

namespace leo
{

class Texture;

class PBRMaterial : public IComponent
{
public:
  PBRMaterial();
  PBRMaterial(bool force);

public:
  virtual ComponentType getTypeId() const override
  {
    return ComponentType::PBR_MATERIAL;
  }

public:
  glm::vec3 albedo_value;
  Texture *albedo_texture = TextureManager::white.get();
  Texture *normal_map = TextureManager::blue.get();
  float metalness_value;
  Texture *metalness_texture = TextureManager::black.get();
  float roughness_value;
  Texture *roughness_texture = TextureManager::black.get();
  Texture *ao_map = TextureManager::white.get();
  Texture *parallax_map = TextureManager::black.get();
  bool force = false;
};

} // namespace leo
