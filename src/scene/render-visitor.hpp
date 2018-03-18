#include "visitor.hpp"

class RenderVisitor : public Visitor {
  public:
    RenderVisitor();
  private:
    virtual void visit(Node *node);
};
