//
// Created by leo on 9/25/16.
//

#include "mesh.hpp"

namespace leo {

  Mesh::Mesh() {
    std::vector<GLfloat> pos {
      1.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 
        0.0f,  1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,

        1.0f, 0.0f,  1.0f, 
        1.0f,  1.0f,  1.0f,
        0.0f,  1.0f,  1.0f, 
        0.0f, 0.0f,  1.0f, 

        0.0f,  1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f,  1.0f,
        0.0f,  1.0f,  1.0f,

        1.0f,  1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f,  1.0f,
        1.0f,  1.0f,  1.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f,  1.0f,
        0.0f, 0.0f,  1.0f,
        0.0f, 0.0f, 0.0f,

        1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f,
        0.0f,  1.0f,  1.0f,
        0.0f,  1.0f, 0.0f,
    };
    std::vector<GLfloat> norm{
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    for (int i = 0; i < 6 * 4 * 3; i+=3) {
      struct Vertex v;
      v.position = glm::vec3(pos[i], pos[i + 1], pos[i + 2]);
      v.normal = glm::vec3(norm[i], norm[i + 1], norm[i + 2]);
      v.texCoords = glm::vec2(0.0f, 1.0f);
      this->_vertices.push_back(v);
    }

    this->_indices = std::vector<GLuint>{
      0, 1, 3,
        1, 2, 3,
        4, 5, 7,
        5, 6, 7,
        8, 9, 11,
        9, 10, 11,
        12, 13, 15,
        13, 14, 15,
        16, 17, 19,
        17, 18, 19,
        20, 21, 23,
        21, 22, 23
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

    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for (GLuint i = 0; i < this->_textures.size(); i++) {
      glActiveTexture(GL_TEXTURE0 + i);
      // Retrieve texture number (the N in diffuse_textureN)
      std::stringstream ss;
      std::string number;
      std::string name = this->_textures[i].type;
      if (name == "texture_diffuse")
        ss << diffuseNr++; // Transfer GLuint to stream
      else if (name == "texture_specular")
        ss << specularNr++; // Transfer GLuint to stream
      number = ss.str();

      glUniform1f(glGetUniformLocation(material->getProgram(),
            ("material." + name + number).c_str()),
          i);
      glBindTexture(GL_TEXTURE_2D, this->_textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // Draw mesh
    glBindVertexArray(this->_VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei) this->_indices.size(),
        GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

  }

}
