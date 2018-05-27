//
// Created by leo on 9/25/16.
//

#pragma once

#include <global.hpp>
#include <scene/geometry-node.hpp>
#include <material/material.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <vector>
#include <assimp/types.h>

namespace leo {

  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
  };

  struct Texture {
    GLuint id;
    std::string type;
    aiString path;
  };

  class Mesh : public GeometryNode {
    public:
      Mesh();
      Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices,
          std::vector<Texture> textures);

    public:
      virtual void draw(Material *material) override;
      virtual void update(double delta) override { UNUSED(delta); };

    private:
      void setupMesh();

    private:
      std::vector<Vertex> _vertices;
      std::vector<GLuint> _indices;
      std::vector<Texture> _textures;
      GLuint _VAO;
      GLuint _VBO;
      GLuint _EBO;
  };

}

