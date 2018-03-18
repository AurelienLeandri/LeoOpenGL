#include "node.hpp"

class Visitor {
  public:
    Visitor();
  protected:
    virtual void visit(Node *node) = 0;
};
