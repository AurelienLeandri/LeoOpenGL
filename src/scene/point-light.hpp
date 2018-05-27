#pragma once

#include <scene/light.hpp>

static UPointLight : public ULight {
  glm::vec3 position;
}

namespace leo {

  class PointLight : public Light {
    public:
      virtual void draw(Material *material) {};
      virtual void update(double delta) = 0;

    public:
      void transform(const glm::mat4x4 &transformation);
      UPointLight createLightUniform();

    private:
      glm::vec3 _position;
  }

}
