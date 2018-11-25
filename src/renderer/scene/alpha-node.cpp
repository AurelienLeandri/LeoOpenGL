#include "alpha-node.hpp"

namespace leo {

  AlphaNode::AlphaNode() : Node() {
  }

  AlphaNode::~AlphaNode() {
  }

  void AlphaNode::addSortedChild(int rank, GeometryNode *child) {
    this->_sortedChildren.insert(std::pair<int, GeometryNode*>(rank, child));
  }

  const std::map<int, GeometryNode*> &AlphaNode::getSortedChildren() {
    return this->_sortedChildren;
  }

}
