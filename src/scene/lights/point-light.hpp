#pragma once

#include <scene/lights/light.hpp>
#include <scene/mesh.hpp>
#include <scene/visitor/transformation-visitor.hpp>

namespace leo {
  class PointLight;

  typedef struct UPointLight : public ULight {
    UPointLight(const PointLight *light);
    glm::vec3 position;
  } UPointLight;

  class PointLight : public Light {
    public:
      PointLight(bool genMesh);
      PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool genMesh);
      PointLight(float constant, float linear, float quadratic,
          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool genMesh);
      virtual ~PointLight() {}
    public:
      virtual void draw(Shader *shader) override {};
      virtual void update(double delta) override {};

    public:
      void transform(const glm::mat4x4 &transformation);
      UPointLight createLightUniform();

    private:
      void _genMesh();

    private:
      glm::vec3 _position;

      friend UPointLight::UPointLight(const PointLight *light);
  };

}
