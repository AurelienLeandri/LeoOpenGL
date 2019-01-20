#include "point-light.hpp"

namespace leo {
namespace model {

  PointLightUniform::PointLightUniform() {}

  PointLightUniform::PointLightUniform(const PointLight &light) :
    LightUniform(light)
  {
    this->position = light._position;
  }

  PointLightUniform::~PointLightUniform() {}

  PointLightUniform::PointLightUniform(const PointLightUniform &other) :
    LightUniform::LightUniform(other)
  {
    this->position = other.position;
  }

  PointLightUniform &PointLightUniform::operator=(const PointLightUniform &other) {
    LightUniform::operator=(other);
    this->position = other.position;
    return *this;
  }

  PointLight::PointLight() :
    Light()
  {
    this->_position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  }

  PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    Light(ambient, diffuse, specular)
  {
    this->_position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  }

  PointLight::PointLight(float constant, float linear, float quadratic,
      glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    Light(constant, linear, quadratic, ambient, diffuse, specular)
  {
    this->_position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  }

  PointLight::PointLight(const PointLight &other) :
    Light(other),
    _position(other._position)
  {
  }

  PointLight &PointLight::operator=(const PointLight &other) {
    Light::operator=(other);
    this->_position = other._position;
    return *this;
  }

  glm::vec4 PointLight::getTransformedPosition(const glm::mat4x4 &transformation) {
    return transformation * this->_position;
  }

}  // namespace model
}  // namespace leo
