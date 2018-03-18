#pragma once

#include "node.hpp"

namespace leo {

class Visitor {
  public:
    Visitor();
  public:
    virtual void visit(Node *node) = 0;
};

}
