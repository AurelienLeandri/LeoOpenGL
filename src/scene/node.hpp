#include <vector>

class Node {
  public:
    Node();
    virtual void update(double delta) = 0;
  protected:
    std::vector<Node*> _children;
    
};
