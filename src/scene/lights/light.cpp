#include <scene/lights/light.hpp>

namespace leo {

  Light::Light() : Light(
      //glm::vec3(1.0f, 1.0f, 1.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(1.0f, 1.0f, 1.0f),
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

  ULight::ULight(const Light *light)
    : constant(light->_constant),
    linear(light->_linear),
    quadratic(light->_quadratic),
    ambient(light->_ambient),
    diffuse(light->_diffuse),
    specular(light->_specular)
  {
  }

}
