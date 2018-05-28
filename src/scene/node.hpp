#pragma once

#include <vector>

namespace leo {

class Node {
  public:
    Node();
    virtual ~Node();

  public:
    virtual void update(double delta) = 0;
    virtual void addChild(Node *child);

  private:
    void _destroy();

  public:
    const std::vector<Node*> &getChildren();

  protected:
    std::vector<Node*> _children;
    
};

}
