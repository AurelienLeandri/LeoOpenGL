#pragma once

#include <scene/lights/light.hpp>
#include <scene/mesh.hpp>
#include <scene/visitor/transformation-visitor.hpp>

namespace leo {
  class DirectionLight;

  typedef struct UDirectionLight : public ULight {
    UDirectionLight(const DirectionLight *light);
    glm::vec3 direction;
  } UDirectionLight;

  class DirectionLight : public Light {
    public:
      DirectionLight();
      DirectionLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
      DirectionLight(float constant, float linear, float quadratic,
          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
      virtual ~DirectionLight() {}
    public:
      virtual void draw(Shader *shader) override {};
      virtual void update(double delta) override {};

    public:
      void transform(const glm::mat4x4 &transformation);
      UDirectionLight createLightUniform();

    private:
      glm::vec3 _direction;

      friend UDirectionLight::UDirectionLight(const DirectionLight *light);
  };

}
