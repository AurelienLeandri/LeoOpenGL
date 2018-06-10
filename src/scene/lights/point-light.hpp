#pragma once

#include <scene/lights/light.hpp>
#include <scene/mesh.hpp>
#include <scene/visitor/transformation-visitor.hpp>

namespace leo {

  typedef struct UPointLight {
      glm::vec3 ambient;
      float constant;
      glm::vec3 diffuse;
      float linear;
      glm::vec3 specular;
      float quadratic;
      glm::vec4 position;
  } UPointLight;

  class PointLight : public Light {
    public:
      PointLight(bool genMesh);
      PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool genMesh);
      PointLight(float constant, float linear, float quadratic,
          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool genMesh);
      virtual ~PointLight() {}
      PointLight(const PointLight &other);
      PointLight &operator=(const PointLight &other);

    public:
      virtual void draw(Shader *shader) override {};
      virtual void update(double delta) override {};

    public:
      virtual void transform(const glm::mat4x4 &transformation) override;
      UPointLight createLightUniform();

    private:
      void _generateMesh();

    private:
      glm::vec4 _position;
      bool _genMesh;

    public:
      const glm::vec4 &getPosition() { return this->_position; }
      void getPosition(const glm::vec4 &value) { this->_position = value; }
  };

}
