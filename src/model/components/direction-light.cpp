#include "direction-light.hpp"

namespace leo {
  namespace model {

  DirectionLight::DirectionLight() : Light() {
    this->direction = glm::vec4(0.0, -1.0, 0.0, 0.0);
  }

  DirectionLight::DirectionLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    Light(ambient, diffuse, specular)
  {
    this->direction = glm::vec4(0.0, -1.0, 0.0, 0.0);
  }

  DirectionLight::DirectionLight(float constant, float linear, float quadratic,
      glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    Light(constant, linear, quadratic, ambient, diffuse, specular)
  {
    this->direction = glm::vec4(0.0, -1.0, 0.0, 0.0);
  }

}  // namespace model
}  // namespace leo
