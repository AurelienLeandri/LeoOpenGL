#pragma once

#include <global.hpp>

#include <scene/geometry-node.hpp>
#include <scene/mesh.hpp>

namespace leo {

  class Model: public GeometryNode {
    public:
      Model();
      virtual ~Model();
      virtual void draw(Material *material) override;
      virtual void update(double delta) override { UNUSED(delta); }

    private:
      std::vector<Mesh> _meshes;

  };

}
