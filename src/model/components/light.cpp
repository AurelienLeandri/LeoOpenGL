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
    _constant(constant),
    _linear(linear),
    _quadratic(quadratic),
    _ambient(ambient),
    _diffuse(diffuse),
    _specular(specular)
  {}

  Light::Light(const Light &other) :
    Component(other),
    _constant(other._constant),
    _linear(other._linear),
    _quadratic(other._quadratic),
    _ambient(other._ambient),
    _diffuse(other._diffuse),
    _specular(other._specular)
  {
  }

  Light &Light::operator=(const Light &other) {
    Component::operator=(other);
    this->_constant = other._constant;
    this->_linear = other._linear;
    this->_quadratic = other._quadratic;
    this->_ambient = other._ambient;
    this->_diffuse = other._diffuse;
    this->_specular = other._specular;
    return *this;
  }

}  // namespace model
}  // namespace leo
