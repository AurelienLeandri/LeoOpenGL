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
      virtual ~PointLight() {}
      PointLight(const PointLight &other);
      PointLight &operator=(const PointLight &other);

    public:
      glm::vec4 getTransformedPosition(const glm::mat4x4 &transformation);

    private:
      void _generateMesh();

    private:
      glm::vec4 _position;
      bool _genMesh;

    public:
      const glm::vec3 &getPosition() { return this->_position; }
      void getPosition(const glm::vec4 &value) { this->_position = value; }

    friend renderer::PointLightUniform::PointLightUniform(const PointLight &light);
  };

  }  // namespace model
}  // namespace leo
