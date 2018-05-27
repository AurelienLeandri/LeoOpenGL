#pragma once

#include <scene/light.hpp>

namespace leo {
  class PointLight;

  typedef struct UPointLight : public ULight {
    UPointLight(const PointLight *light);
    glm::vec3 position;
  } UPointLight;

  class PointLight : public Light {
    public:
      virtual void draw(Material *material) {};
      virtual void update(double delta) = 0;

    public:
      void transform(const glm::mat4x4 &transformation);
      UPointLight createLightUniform();

    private:
      glm::vec3 _position;

      friend UPointLight::UPointLight(const PointLight *light);
  };

}
