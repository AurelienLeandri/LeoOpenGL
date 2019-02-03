#pragma once

#include <renderer/global.hpp>
#include <model/component.hpp>

namespace leo {
namespace model {

  class Light : public Component {
    public:
      Light();
      Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
      Light(float constant, float linear, float quadratic,
          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    public:
      float constant = 1.f;
      float linear = 0.09f;
      float quadratic = 0.032;
      glm::vec3 ambient;
      glm::vec3 diffuse;
      glm::vec3 specular;

  };

}  // namespace model
}  // namespace leo
