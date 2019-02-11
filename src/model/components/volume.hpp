#pragma once

#include <model/component.hpp>

#include <utils/geometry.hpp>

#include <vector>

namespace leo {
  namespace model {

    class Volume: public Component<Volume> {
      public:
        Volume(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

      private:
        Volume();

      public:
        const std::vector<Vertex> &getVertices() const;
        const std::vector<unsigned int> &getIndices() const;

      public:
        static Volume createCustom(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        static Volume createCube(float side);
        static Volume createPlane(float width, float height);

      private:
        std::vector<Vertex> _vertices;
        std::vector<unsigned int> _indices;
    };

  }  // namespace model
}  // namespace leo
