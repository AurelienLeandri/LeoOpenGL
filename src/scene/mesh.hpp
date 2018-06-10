//
// Created by leo on 9/25/16.
//

#pragma once

#include <global.hpp>
#include <scene/geometry-node.hpp>
#include <utils/shader.hpp>
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
      Mesh(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLint shininess);
      Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices,
          std::vector<Texture> textures,
          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLint shininess);
      virtual ~Mesh();
      Mesh(const Mesh &other);
      Mesh &operator=(const Mesh &other);

    public:
      virtual void draw(Shader *shader) override;
      virtual void update(double delta) override { UNUSED(delta); };

    private:
      void _setupMesh();
      void _generateDefaultMesh();

    private:
      std::vector<Vertex> _vertices;
      std::vector<GLuint> _indices;
      std::vector<Texture> _textures;
      glm::vec3 _ambient;
      glm::vec3 _diffuse;
      glm::vec3 _specular;
      GLuint _VAO;
      GLuint _VBO;
      GLuint _EBO;
      GLint _shininess;
  };

}
