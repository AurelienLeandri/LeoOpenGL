#pragma once

#include <renderer/scene/node.hpp>
#include <renderer/global.hpp>

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
