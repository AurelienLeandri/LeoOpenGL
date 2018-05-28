#include <scene/lights/point-light.hpp>

namespace leo {

  UPointLight::UPointLight(const PointLight *light)
    : ULight(light), position(light->_position)
  {}

  UPointLight PointLight::createLightUniform() {
    return UPointLight(this);
  }

}
