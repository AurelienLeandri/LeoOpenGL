#include "light.hpp"

namespace leo {
namespace model {

  Light::Light() : Light(
      glm::vec3(0.2f, 0.2f, 0.2f),
      glm::vec3(0.5f, 0.5f, 0.5f),
      glm::vec3(1.0f, 1.0f, 1.0f)
      )
  {}

  Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Light(
      1.0f, 0.09f, 0.032f,
      ambient,
      diffuse,
      specular
      )
  {}

  Light::Light(float constant, float linear, float quadratic,
          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    constant(constant),
    linear(linear),
    quadratic(quadratic),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular)
  {}

  Light::~Light() {
  }

}  // namespace model
}  // namespace leo
