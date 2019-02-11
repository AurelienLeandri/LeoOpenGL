#pragma once

#include <model/components/light.hpp>

namespace leo {
namespace model {

  class DirectionLight : public Light, public Component<DirectionLight> {
    public:
      DirectionLight();
      DirectionLight(glm::vec3 ambient, glm::vec3 diffuse,
          glm::vec3 specular);
      DirectionLight(float constant, float linear, float quadratic,
          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    public:
      glm::vec4 direction;
  };

}  // namespace model
}  // namespace leo