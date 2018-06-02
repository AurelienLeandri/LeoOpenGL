#include <scene/lights/direction-light.hpp>

namespace leo {

  // TODO: refactor reduntant code

  UDirectionLight::UDirectionLight(const DirectionLight *light) :
    ULight(light),
    direction(light->_modelMatrix * glm::vec4(light->_direction, 0.0))
  {
  }

  UDirectionLight DirectionLight::createLightUniform() {
    return UDirectionLight(this);
  }

  DirectionLight::DirectionLight() : Light() {
    this->_direction = glm::vec3(0.0, -1.0, 0.0);
  }

  DirectionLight::DirectionLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    Light(ambient, diffuse, specular) {
      this->_direction = glm::vec3(0.0, -1.0, 0.0);
    }

  DirectionLight::DirectionLight(float constant, float linear, float quadratic,
      glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    Light(constant, linear, quadratic, ambient, diffuse, specular) {
      this->_direction = glm::vec3(0.0, -1.0, 0.0);
    }

}
