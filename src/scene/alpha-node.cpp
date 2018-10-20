#include <scene/alpha-node.hpp>

namespace leo {

  AlphaNode::AlphaNode() : DrawableNode() {
  }

  AlphaNode::~AlphaNode() {
  }

  void AlphaNode::draw(Shader *shader) {
    for (auto &e: this->_sortedChildren) {
      e.second->draw(shader);
    }
  }

  void AlphaNode::addSortedChild(int rank, GeometryNode *child) {
    this->_sortedChildren.insert(std::pair<int, GeometryNode*>(rank, child));
  }

}
