#pragma once

#include <scene/geometry-node.hpp>
#include <map>

namespace leo {
  class AlphaNode : public DrawableNode {
    public:
      AlphaNode();
      virtual ~AlphaNode();
      AlphaNode(const AlphaNode &other) = delete;
      AlphaNode &operator=(const AlphaNode &other) = delete;

    public:
      void draw(Shader *shader) override;
      void addSortedChild(int rank, GeometryNode *child);

    private:
      std::map<int, GeometryNode*> _sortedChildren;

  };
}
