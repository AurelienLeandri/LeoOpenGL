#include "node.hpp"

Node::Node() {
}

void Node::addChild(Node *child) {
  this->_children.push_back(child);
}

const std::vector<Node*> &Node::getChildren() {
  return this->_children;
}
