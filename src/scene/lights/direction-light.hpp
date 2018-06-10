#pragma once

#include <scene/lights/light.hpp>
#include <scene/mesh.hpp>
#include <scene/visitor/transformation-visitor.hpp>

namespace leo {

  typedef struct UDirectionLight {
    glm::vec3 ambient;
    float constant;
    glm::vec3 diffuse;
    float linear;
    glm::vec3 specular;
    float quadratic;
    glm::vec4 direction;
  } UDirectionLight;

  class DirectionLight : public Light {
    public:
      DirectionLight();
      DirectionLight(glm::vec3 ambient, glm::vec3 diffuse,
          glm::vec3 specular);
      DirectionLight(float constant, float linear, float quadratic,
          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
      virtual ~DirectionLight() {}
      DirectionLight(const DirectionLight &other);
      DirectionLight &operator=(const DirectionLight &other);
    public:
      virtual void draw(Shader *shader) override { UNUSED(shader); };
      virtual void update(double delta) override { UNUSED(delta); };

    public:
      virtual void transform(const glm::mat4x4 &transformation) override;
      UDirectionLight createLightUniform();

    private:
      glm::vec4 _direction;

    public:
      const glm::vec4 &getDirection() { return this->_direction; }
      void getDirection(const glm::vec4 &value) { this->_direction = value; }
  };

}
