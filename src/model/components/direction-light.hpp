#pragma once

#include <model/components/light.hpp>
#include <model/icomponent.hpp>

namespace leo {
namespace model {

  class DirectionLight : public Light, public IComponent {
    public:
      DirectionLight();
      DirectionLight(glm::vec3 ambient, glm::vec3 diffuse,
          glm::vec3 specular);
      DirectionLight(float constant, float linear, float quadratic,
          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    public:
      virtual ComponentType getTypeId() const override {
        return ComponentType::DIRECTION_LIGHT;
      }

    public:
      glm::vec4 direction;
  };

}  // namespace model
}  // namespace leo