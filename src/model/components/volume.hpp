#pragma once

#include <model/components/types/drawable.hpp>
#include <vector>
#include <renderer/utils/geometry.hpp>

namespace leo {
  namespace model {

    class Volume: public Drawable {
      public:
        Volume(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        virtual ~Volume();
        Volume(const Volume &other);
        virtual const Volume &operator=(const Volume &other);

      private:
        Volume();

      public:
        const std::vector<Vertex> &getVertices() const;
        const std::vector<unsigned int> &getIndices() const;

      public:
        static Volume createCustom(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        static Volume createCube(float side);

      private:
        std::vector<Vertex> _vertices;
        std::vector<unsigned int> _indices;
    };

  }  // namespace model
}  // namespace leo
