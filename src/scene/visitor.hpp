#pragma once

#include "node.hpp"

class Visitor {
  public:
    Visitor();
  public:
    virtual void visit(Node *node) = 0;
};
