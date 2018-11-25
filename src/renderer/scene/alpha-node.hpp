#pragma once

#include <renderer/scene/geometry-node.hpp>
#include <map>

namespace leo {
  class AlphaNode : public Node {
    public:
      AlphaNode();
      virtual ~AlphaNode();
      AlphaNode(const AlphaNode &other) = delete;
      AlphaNode &operator=(const AlphaNode &other) = delete;

    public:
      void addSortedChild(int rank, GeometryNode *child);
      const std::map<int, GeometryNode*> &getSortedChildren();

    private:
      std::map<int, GeometryNode*> _sortedChildren;

  };
}
