#include "point-light.hpp"

namespace leo {

  PointLight::PointLight() :
    Light()
  {
    this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  }

  PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    Light(ambient, diffuse, specular)
  {
    this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  }

  PointLight::PointLight(float constant, float linear, float quadratic,
      glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    Light(constant, linear, quadratic, ambient, diffuse, specular)
  {
    this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  }

}  // namespace leo
