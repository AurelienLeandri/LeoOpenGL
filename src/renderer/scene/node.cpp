#include "node.hpp"

namespace leo {

Node::Node() {
}

Node::~Node() {
  this->_destroyChildren();
}

void Node::addChild(Node *child) {
  this->_children.push_back(child);
}

void Node::_destroyChildren() {
  for (auto *e : this->_children)
    delete e;
}

const std::vector<Node*> &Node::getChildren() {
  return this->_children;
}

}
