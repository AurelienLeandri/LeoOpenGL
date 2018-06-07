#pragma once

#include <global.hpp>

#include <scene/geometry-node.hpp>

namespace leo {

  class Light : public GeometryNode {
    public:
      Light();
      Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
      Light(float constant, float linear, float quadratic,
          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
      virtual ~Light() {}

    public:
      virtual void draw(Shader *shader) {};
      virtual void update(double delta) = 0;

    protected:
      float _constant;
      float _linear;
      float _quadratic;
      glm::vec3 _ambient;
      glm::vec3 _diffuse;
      glm::vec3 _specular;
  };

}
