#pragma once

#include <renderer/global.hpp>

#include <renderer/scene/geometry-node.hpp>

namespace leo {
  class Light;

  typedef struct LightUniform {
    LightUniform();
    LightUniform(const Light &light);
    ~LightUniform();
    LightUniform(const LightUniform &other);
    LightUniform &operator=(const LightUniform &other);
    glm::vec3 ambient;
    float constant;
    glm::vec3 diffuse;
    float linear;
    glm::vec3 specular;
    float quadratic;
  } LightUniform;

  class Light : public GeometryNode {
    public:
      Light();
      Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
      Light(float constant, float linear, float quadratic,
          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
      virtual ~Light() {}
      Light(const Light &other);
      Light &operator=(const Light &other);

    public:
      virtual void draw(Shader *shader) = 0;
      virtual void update(double delta) = 0;

    protected:
      float _constant;
      float _linear;
      float _quadratic;
      glm::vec3 _ambient;
      glm::vec3 _diffuse;
      glm::vec3 _specular;

    public:
      const float &getConstant() const { return this->_constant; }
      const float &getLinear() const { return this->_linear; }
      const float &getQuadratic() const { return this->_quadratic; }
      const glm::vec3 &getAmbient() const { return this->_ambient; }
      const glm::vec3 &getDiffuse() const { return this->_diffuse; }
      const glm::vec3 &getSpecular() const { return this->_specular; }
      void setConstant(const float &value) { this->_constant = value; }
      void setLinear(const float &value) { this->_linear = value; }
      void setQuadratic(const float &value) { this->_quadratic = value; }
      void setAmbient(const glm::vec3 &value) { this->_ambient = value; }
      void setDiffuse(const glm::vec3 &value) { this->_diffuse = value; }
      void setSpecular(const glm::vec3 &value) { this->_specular = value; }

      friend LightUniform::LightUniform(const Light &light);

  };

}
