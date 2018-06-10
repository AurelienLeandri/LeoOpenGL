#include <scene/lights/direction-light.hpp>

namespace leo {

  DirectionLightUniform::DirectionLightUniform() {}

  DirectionLightUniform::DirectionLightUniform(const DirectionLight &light) :
    LightUniform(light)
  {
    this->direction = light._direction;
  }

  DirectionLightUniform::~DirectionLightUniform() {}

  DirectionLightUniform::DirectionLightUniform(const DirectionLightUniform &other) :
    LightUniform::LightUniform(other)
  {
    this->direction = other.direction;
  }

  DirectionLightUniform &DirectionLightUniform::operator=(const DirectionLightUniform &other) {
    LightUniform::operator=(other);
    this->direction = other.direction;
    return *this;
  }

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

  void DirectionLight::transform(const glm::mat4x4 &transformation) {
    GeometryNode::transform(transformation);
    this->_direction = transformation * this->_direction;
  }

}
