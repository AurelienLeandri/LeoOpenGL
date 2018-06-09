#include <scene/lights/direction-light.hpp>

namespace leo {

  // TODO: refactor reduntant code

  UDirectionLight DirectionLight::createLightUniform() {
    UDirectionLight light;
    light.ambient = this->_ambient;
    light.constant = this->_constant;
    light.diffuse = this->_diffuse;
    light.linear = this->_linear;
    light.specular = this->_specular;
    light.quadratic = this->_quadratic;
    light.direction = this->_direction;
    return light;
  }

  DirectionLight::DirectionLight() : Light() {
    this->_direction = glm::vec4(0.0, -1.0, 0.0, 0.0);
  }

  DirectionLight::DirectionLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    Light(ambient, diffuse, specular) {
      this->_direction = glm::vec4(0.0, -1.0, 0.0, 0.0);
    }

  DirectionLight::DirectionLight(float constant, float linear, float quadratic,
      glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    Light(constant, linear, quadratic, ambient, diffuse, specular) {
      this->_direction = glm::vec4(0.0, -1.0, 0.0, 0.0);
    }

  void DirectionLight::transform(const glm::mat4x4 &transformation) {
    GeometryNode::transform(transformation);
    this->_direction = transformation * this->_direction;
  }

}
