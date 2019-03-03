#include "material.hpp"

namespace leo {
  namespace model {

    Material::Material() : Material(false) {
    }
    

    Material::Material(bool force) : force(force), diffuse_value(0.f, 0.f, 0.f), specular_value(0.f, 0.f, 0.f)
    {
    }

  }  // namespace model
}  // namespace leo
