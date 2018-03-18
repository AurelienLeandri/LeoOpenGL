#include "node.hpp"

Node::Node() {
}

Node::~Node() {
  this->_destroy();
}

void Node::addChild(Node *child) {
  this->_children.push_back(child);
}

void Node::_destroy() {
  for (auto *e : this->_children)
    delete e;
}

const std::vector<Node*> &Node::getChildren() {
  return this->_children;
}
