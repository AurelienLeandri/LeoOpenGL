#pragma once

#include <model/components/light.hpp>
#include <renderer/light-uniforms.hpp>

namespace leo {
  namespace model {

  class PointLight : public Light {
    public:
      PointLight();
      PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
      PointLight(float constant, float linear, float quadratic,
          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    public:
      glm::vec4 getTransformedPosition(const glm::mat4x4 &transformation);

    private:
      glm::vec4 _position;

    friend renderer::PointLightUniform::PointLightUniform(const PointLight &light);
  };

  }  // namespace model
}  // namespace leo
