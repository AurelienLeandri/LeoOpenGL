#pragma once

#include <scene/node.hpp>

namespace leo {

class Visitor {
  public:
    Visitor();
    virtual ~Visitor();
    Visitor(const Visitor &other);
    Visitor &operator=(const Visitor &other);
  public:
    virtual void visit(Node *node) = 0;
};

}
