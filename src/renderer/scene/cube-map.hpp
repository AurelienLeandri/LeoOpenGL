#pragma once

#include <renderer/scene/geometry-node.hpp>
#include <renderer/utils/shader.hpp>
#include <renderer/scene/mesh.hpp>
#include <string>

namespace leo {

  class CubeMap : public GeometryNode {
  public:
    CubeMap(std::string name, std::string path);
    virtual ~CubeMap();
    CubeMap(const CubeMap &other) = delete;
    CubeMap &operator=(const CubeMap &other) = delete;

  public:
    virtual void draw(Shader *shader);
    virtual void update(double delta) { UNUSED(delta); }

  public:
    const Texture &getTexture() const { return this->_texture; }

    private:
      Texture _texture;
      GLuint _VAO;
      GLuint _VBO;
  };

}
