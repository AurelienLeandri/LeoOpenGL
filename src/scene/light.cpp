#include <scene/light.hpp>

namespace leo {

  ULight::ULight(const Light &light)
    : constant(light._constant),
    linear(light._linear),
    quadratic(light._quadratic),
    ambient(light._ambient),
    diffuse(light._diffuse),
    specular(light._specular)
  {
  }

}
