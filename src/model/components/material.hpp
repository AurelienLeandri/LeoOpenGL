#pragma once

#include <model/component.hpp>
#include <renderer/utils/texture.hpp>

namespace leo {
  namespace model {

    class Material : public Component {
      public:
        Material();
        Material(bool force);

      public:
        glm::vec3 diffuse_value;
        std::shared_ptr<Texture> diffuse_texture = nullptr;
        glm::vec3 specular_value;
        std::shared_ptr<Texture> specular_texture = nullptr;
        std::shared_ptr<Texture> reflection_map = nullptr;
        float shininess = 32.f;
        bool force;
    };

  }  // namespace model
}  // namespace leo
