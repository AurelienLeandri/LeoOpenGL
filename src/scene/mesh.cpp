//
// Created by leo on 9/25/16.
//

#include "mesh.hpp"

namespace leo {

  Mesh::Mesh() {
    struct Vertex v1;
    v1.position = glm::vec3(1.0f, 0.0f, 0.0f);
    v1.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    v1.texCoords = glm::vec2(0.0f, 1.0f);
    struct Vertex v2;
    v2.position = glm::vec3(1.0f, 1.0f, 0.0f);
    v2.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    v2.texCoords = glm::vec2(0.0f, 1.0f);
    struct Vertex v3;
    v3.position = glm::vec3(0.0f, 1.0f, 0.0f);
    v3.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    v3.texCoords = glm::vec2(0.0f, 1.0f);
    this->_vertices.push_back(v1);
    this->_vertices.push_back(v2);
    this->_vertices.push_back(v3);

    this->_indices = std::vector<GLuint>{
      0, 1, 2
    };
    this->setupMesh();
  }

  Mesh::Mesh(std::vector<Vertex> vertices,
      std::vector<GLuint> indices,
      std::vector<Texture> textures) :
    _vertices(vertices),
    _indices(indices),
    _textures(textures) {
      this->setupMesh();
    }

  void Mesh::setupMesh() {
    glGenVertexArrays(1, &this->_VAO);
    glGenBuffers(1, &this->_VBO);
    glGenBuffers(1, &this->_EBO);

    glBindVertexArray(this->_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);

    glBufferData(GL_ARRAY_BUFFER, this->_vertices.size() * sizeof(Vertex),
        &this->_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->_indices.size() * sizeof(GLuint),
        &this->_indices[0], GL_STATIC_DRAW);
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (GLvoid *) 0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (GLvoid *) offsetof(Vertex, normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (GLvoid *) offsetof(Vertex, texCoords));

    glBindVertexArray(0);
  }

  void Mesh::draw(Material *material) {
    UNUSED(material);

    // Draw mesh
    glBindVertexArray(this->_VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei) this->_indices.size(),
        GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

  }

}
