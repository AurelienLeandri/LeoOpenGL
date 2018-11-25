#pragma once

#include <renderer/scene/lights/light.hpp>
#include <renderer/scene/mesh.hpp>
#include <renderer/scene/visitor/transformation-visitor.hpp>

namespace leo {

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
      PointLight(bool genMesh);
      PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool genMesh);
      PointLight(float constant, float linear, float quadratic,
          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool genMesh);
      virtual ~PointLight() {}
      PointLight(const PointLight &other);
      PointLight &operator=(const PointLight &other);

    public:
      virtual void draw(Shader *shader) override { UNUSED(shader); };
      virtual void update(double delta) override { UNUSED(delta); };

    public:
      virtual void transform(const glm::mat4x4 &transformation) override;

    private:
      void _generateMesh();

    private:
      glm::vec4 _position;
      bool _genMesh;

    public:
      const glm::vec4 &getPosition() { return this->_position; }
      void getPosition(const glm::vec4 &value) { this->_position = value; }

    friend PointLightUniform::PointLightUniform(const PointLight &light);
  };

}
