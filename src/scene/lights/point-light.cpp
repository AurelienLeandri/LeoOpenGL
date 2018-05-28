#include <scene/lights/point-light.hpp>

namespace leo {

  UPointLight::UPointLight(const PointLight *light) :
    ULight(light),
    position(light->_modelMatrix * glm::vec4(light->_position, 1.0))
  {}

  UPointLight PointLight::createLightUniform() {
    return UPointLight(this);
  }

}
