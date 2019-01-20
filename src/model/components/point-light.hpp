#pragma once

#include <model/components/light.hpp>

namespace leo {
  namespace model {

  class PointLight;

  typedef struct PointLightUniform : public LightUniform {
    PointLightUniform();
    PointLightUniform(const PointLight &light);
    ~PointLightUniform();
    PointLightUniform(const PointLightUniform &other);
    PointLightUniform &operator=(const PointLightUniform &other);
    glm::vec4 position;
  } PointLightUniform;

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

    friend PointLightUniform::PointLightUniform(const PointLight &light);
  };

  }  // namespace model
}  // namespace leo
