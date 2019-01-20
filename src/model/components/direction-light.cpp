#include "direction-light.hpp"

namespace leo {
  namespace model {

  DirectionLight::DirectionLight() : Light() {
    this->_direction = glm::vec4(0.0, -1.0, 0.0, 0.0);
  }

  DirectionLight::DirectionLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    Light(ambient, diffuse, specular)
  {
    this->_direction = glm::vec4(0.0, -1.0, 0.0, 0.0);
  }

  DirectionLight::DirectionLight(float constant, float linear, float quadratic,
      glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    Light(constant, linear, quadratic, ambient, diffuse, specular)
  {
    this->_direction = glm::vec4(0.0, -1.0, 0.0, 0.0);
  }

  DirectionLight::DirectionLight(const DirectionLight &other) :
    Light(other),
    _direction(other._direction)
  {
  }

  DirectionLight &DirectionLight::operator=(const DirectionLight &other) {
    Light::operator=(other);
    this->_direction = other._direction;
    return *this;
  }

  glm::vec4 DirectionLight::getTransformedDirection(const glm::mat4x4 &transformation) {
    return transformation * this->_direction;
  }

}  // namespace model
}  // namespace leo
