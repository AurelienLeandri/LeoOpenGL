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
    public:
      virtual void draw(Shader *shader) override {};
      virtual void update(double delta) override {};

    public:
      virtual void transform(const glm::mat4x4 &transformation) override;
      UDirectionLight createLightUniform();

    private:
      glm::vec4 _direction;
  };

}
