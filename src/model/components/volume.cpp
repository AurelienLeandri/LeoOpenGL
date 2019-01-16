#include "volume.hpp"

namespace leo {
  namespace model {

    Volume::Volume(std::vector<Vertex> vertices, std::vector<unsigned int> indices) :
      _vertices(vertices),
      _indices(indices)
    {
    }

    Volume::~Volume() {
    }

    Volume::Volume(const Volume &other) :
      Volume(other._vertices, other._indices)
    {
    }

    const Volume &Volume::operator=(const Volume &other) {
      this->_vertices = other._vertices;
      this->_indices = other._indices;
      this->_notify(controller::Event::COMPONENT_UPDATED);
      return *this;
    }

    Volume::Volume() {
    }

    const std::vector<Vertex> &Volume::getVertices() const {
      return this->_vertices;
    }
    
    const std::vector<unsigned int> &Volume::getIndices() const {
      return this->_indices;
    }

    Volume Volume::createCustom(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
      Volume volume(vertices, indices);
      return volume;
    }

    Volume Volume::createCube(float side) {
      std::vector<float> pos {
        0.0f, 0.0f, 0.0f,
        0.0f,  side, 0.0f,
        side,  side, 0.0f, 
        side, 0.0f, 0.0f,

        side,  side,  side,
        0.0f,  side,  side, 
        0.0f, 0.0f,  side, 
        side, 0.0f,  side, 

        0.0f,  side, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f,  side,
        0.0f,  side,  side,

        side,  side,  side,
        side, 0.0f,  side,
        side, 0.0f, 0.0f,
        side,  side, 0.0f,

        side, 0.0f, 0.0f,
        side, 0.0f,  side,
        0.0f, 0.0f,  side,
        0.0f, 0.0f, 0.0f,

        0.0f,  side, 0.0f,
        0.0f,  side,  side,
        side,  side,  side,
        side,  side, 0.0f,
      };

      std::vector<float> tex {
        0.0f, 0.0f, 0.0f,
          0.0f, 1.0f, 0.0f,
          1.0f, 1.0f, 0.0f,
          1.0f, 0.0f, 0.0f,

          1.0f, 1.0f, 0.0f,
          0.0f, 1.0f, 0.0f,
          0.0f, 0.0f, 0.0f,
          1.0f, 0.0f, 0.0f,

          1.0f, 0.0f, 0.0f,
          0.0f, 0.0f, 0.0f,
          0.0f, 1.0f, 0.0f,
          1.0f, 1.0f, 0.0f,

          1.0f, 1.0f, 0.0f,
          0.0f, 1.0f, 0.0f,
          0.0f, 0.0f, 0.0f,
          1.0f, 0.0f, 0.0f,

          1.0f, 0.0f, 0.0f,
          1.0f, 1.0f, 0.0f,
          0.0f, 1.0f, 0.0f,
          0.0f, 0.0f, 0.0f,

          0.0f, 0.0f, 0.0f,
          0.0f, 1.0f, 0.0f,
          1.0f, 1.0f, 0.0f,
          1.0f, 0.0f, 0.0f
      };

      std::vector<float> norm {
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
      Volume volume;
      for (int i = 0; i < 6 * 4 * 3; i+=3) {
        struct Vertex v;
        v.position = glm::vec3(pos[i], pos[i + 1], pos[i + 2]);
        v.normal = glm::vec3(norm[i], norm[i + 1], norm[i + 2]);
        v.texCoords = glm::vec2(tex[i], tex[i + 1]);
        volume._vertices.push_back(v);
      }

      volume._indices = std::vector<unsigned int>{
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

      return volume;
    }

  }  // namespace model
}  // namespace leo
