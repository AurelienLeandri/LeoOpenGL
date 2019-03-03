#pragma once

#include <model/components/light.hpp>
#include <model/icomponent.hpp>

namespace leo
{
namespace model
{

class PointLight : public Light, public IComponent
{
public:
  PointLight();
  PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
  PointLight(float constant, float linear, float quadratic,
             glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

public:
  virtual ComponentType getTypeId() const override
  {
    return ComponentType::POINT_LIGHT;
  }

public:
  glm::vec4 position;

};

} // namespace model
} // namespace leo
