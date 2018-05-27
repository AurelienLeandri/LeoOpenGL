#include <scene/point-light.hpp>

UpointLight *PointLight::createLightUniform() {
  UPointLight upl;
  upl.position = this->_position;
}
