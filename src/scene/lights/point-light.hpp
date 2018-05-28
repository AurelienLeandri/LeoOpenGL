#pragma once

#include <scene/lights/light.hpp>

namespace leo {
  class PointLight;

  typedef struct UPointLight : public ULight {
    UPointLight(const PointLight *light);
    glm::vec3 position;
  } UPointLight;

  class PointLight : public Light {
    public:
      PointLight() : Light() {}
      PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
        Light(ambient, diffuse, specular) {}
      PointLight(float constant, float linear, float quadratic,
          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
        Light(constant, linear, quadratic, ambient, diffuse, specular) {}
      virtual ~PointLight() {}
    public:
      virtual void draw(Material *material) override {};
      virtual void update(double delta) override {};

    public:
      void transform(const glm::mat4x4 &transformation);
      UPointLight createLightUniform();

    private:
      glm::vec3 _position;

      friend UPointLight::UPointLight(const PointLight *light);
  };

}
