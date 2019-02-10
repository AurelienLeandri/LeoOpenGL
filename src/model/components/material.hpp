#pragma once

#include <model/component.hpp>

#include <renderer/global.hpp>

namespace leo {

  class Texture;

  namespace model {

    class Material : public Component {
      public:
        Material();
        Material(bool force);

      public:
        glm::vec3 diffuse_value;
        Texture * diffuse_texture = nullptr;
        glm::vec3 specular_value;
        Texture * specular_texture = nullptr;
        Texture * reflection_map = nullptr;
        float shininess = 32.f;
        bool force = false;
    };

  }  // namespace model
}  // namespace leo
