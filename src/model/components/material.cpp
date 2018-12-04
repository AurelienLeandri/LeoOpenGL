#include "material.hpp"

namespace leo {
  namespace model {

    Material::Material() : Material(false) {
    }

    Material::Material(bool force) : force(force) {
    }

  }  // namespace model
}  // namespace leo
