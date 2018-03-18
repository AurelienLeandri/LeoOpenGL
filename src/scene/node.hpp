#include <vector>

class Node {
  public:
    Node();

  public:
    virtual void update(double delta) = 0;
    void addChild(Node *child);

  public:
    const std::vector<Node*> &getChildren();

  protected:
    std::vector<Node*> _children;
    
};
