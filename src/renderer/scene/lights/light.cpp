#include "light.hpp"

namespace leo {

  LightUniform::LightUniform() {}

  LightUniform::LightUniform(const Light &light) {
    this->ambient = light._ambient;
    this->constant = light._constant;
    this->diffuse = light._diffuse;
    this->linear = light._linear;
    this->specular = light._specular;
    this->quadratic = light._quadratic;
  }

  LightUniform::~LightUniform() {}

  LightUniform::LightUniform(const LightUniform &other) :
    ambient(other.ambient),
    constant(other.constant),
    diffuse(other.diffuse),
    linear(other.linear),
    specular(other.specular),
    quadratic(other.quadratic)
  {
  }

  LightUniform &LightUniform::operator=(const LightUniform &other) {
    this->ambient = other.ambient;
    this->constant = other.constant;
    this->diffuse = other.diffuse;
    this->linear = other.linear;
    this->specular = other.specular;
    this->quadratic = other.quadratic;
    return *this;
  }

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
    GeometryNode(other),
    _constant(other._constant),
    _linear(other._linear),
    _quadratic(other._quadratic),
    _ambient(other._ambient),
    _diffuse(other._diffuse),
    _specular(other._specular)
  {
  }

  Light &Light::operator=(const Light &other) {
    GeometryNode::operator=(other);
    this->_constant = other._constant;
    this->_linear = other._linear;
    this->_quadratic = other._quadratic;
    this->_ambient = other._ambient;
    this->_diffuse = other._diffuse;
    this->_specular = other._specular;
    return *this;
  }

}
