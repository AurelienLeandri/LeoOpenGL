//
// Created by leo on 9/25/16.
//

#include "mesh.hpp"

namespace leo {

  // TODO: refactor redundant code

  Mesh::Mesh() {

    this->_ambient = glm::vec3(1.0, 1.0, 1.0);
    this->_diffuse = glm::vec3(1.0, 1.0, 1.0);
    this->_specular = glm::vec3(1.0, 1.0, 1.0);
    this->_shininess = 32;
    this->_setupMesh();
  }

  Mesh::Mesh(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLint shininess)
  {
    this->_ambient = ambient;
    this->_diffuse = diffuse;
    this->_specular = specular;
    this->_shininess = shininess;
    this->_generateDefaultMesh();
    this->_setupMesh();
  }

  Mesh::Mesh(std::vector<Vertex> vertices,
      std::vector<GLuint> indices,
      std::vector<Texture> textures) :
    Mesh(vertices, indices, textures,
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 0.0, 0.0),
        32
        )
  {
  }

  Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices,
      std::vector<Texture> textures,
      glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLint shininess) :
    _vertices(vertices),
    _indices(indices),
    _textures(textures),
    _ambient(ambient),
    _diffuse(diffuse),
    _specular(specular),
    _shininess(shininess)
  {
    this->_setupMesh();
  }

  Mesh::~Mesh() {
  }

  Mesh::Mesh(const Mesh &other) :
    GeometryNode(other),
    _vertices(other._vertices),
    _indices(other._indices),
    _textures(other._textures),
    _ambient(other._ambient),
    _diffuse(other._diffuse),
    _specular(other._specular),
    _shininess(other._shininess)
  {
    this->_setupMesh();
  }

  Mesh &Mesh::operator=(const Mesh &other) {
    GeometryNode::operator=(other);
    this->_vertices = other._vertices;
    this->_indices = other._indices;
    this->_textures = other._textures;
    this->_ambient = other._ambient;
    this->_diffuse = other._diffuse;
    this->_specular = other._specular;
    this->_shininess = other._shininess;
    this->_setupMesh();
    return *this;
  }

  void Mesh::_setupMesh() {
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

  void Mesh::draw(Shader *shader) {
    UNUSED(shader);

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

      glUniform1f(glGetUniformLocation(shader->getProgram(),
            ("material." + name + number).c_str()),
          i);
      glBindTexture(GL_TEXTURE_2D, this->_textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glUniform3f(glGetUniformLocation(shader->getProgram(), "material.ambient"),
          this->_ambient.x, this->_ambient.y, this->_ambient.z);
    glUniform3f(glGetUniformLocation(shader->getProgram(), "material.diffuse"),
          this->_diffuse.x, this->_diffuse.y, this->_diffuse.z);
    glUniform3f(glGetUniformLocation(shader->getProgram(), "material.specular"),
          this->_specular.x, this->_specular.y, this->_specular.z);
    glUniform1i(glGetUniformLocation(shader->getProgram(), "material.shininess"),
          this->_shininess);

    // Draw mesh
    glBindVertexArray(this->_VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei) this->_indices.size(),
        GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

  }

  void Mesh::_generateDefaultMesh() {
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
  }

  Mesh *Mesh::createPlaneMesh() {
    Mesh *m = new Mesh();

    std::vector<GLfloat> pos {
      1.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 
        0.0f,  1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
    };

    std::vector<GLfloat> norm{
      0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
    };

    for (int i = 0; i < 4 * 3; i+=3) {
      struct Vertex v;
      v.position = glm::vec3(pos[i], pos[i + 1], pos[i + 2]);
      v.normal = glm::vec3(norm[i], norm[i + 1], norm[i + 2]);
      v.texCoords = glm::vec2(pos[i], pos[i + 1]);
      m->_vertices.push_back(v);
    }

    m->_indices = std::vector<GLuint>{
      0, 1, 3, 1, 2, 3,
    };

    m->_setupMesh();

    return m;
  }

}
