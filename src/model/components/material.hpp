#pragma once

#include <model/component.hpp>
#include <renderer/utils/texture.hpp>

namespace leo {
  namespace model {

    class Material : public Component {
      public:
        Material();
        Material(bool force);
        Material(const Material &other) = delete;
        const Material &operator=(const Material &other) = delete;

      public:
        glm::vec3 diffuse_value;
        Texture *diffuse_texture = nullptr;
        glm::vec3 specular_value;
        Texture *specular_texture = nullptr;
        Texture *reflection_map = nullptr;
        float shininess = 32.f;
        bool force;
    };

  }  // namespace model
}  // namespace leo
