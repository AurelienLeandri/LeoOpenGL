#pragma once

#include <vector>
#include <map>

namespace leo {

class GeometryNode;

class Node {
  public:
    Node();
    virtual ~Node();

  public:
    virtual void update(double delta) = 0;
    virtual void addChild(Node *child);

  private:
    void _destroyChildren();

  public:
    const std::vector<Node*> &getChildren();
    const std::map<unsigned int, GeometryNode*> getTransparentChildren() const {
      return _transparentChildren;
    }
    void addTransparentChild(unsigned int rank, GeometryNode *child) {
      this->_transparentChildren.insert(std::pair<unsigned int, GeometryNode *>(rank, child));
    }

  protected:
    std::vector<Node*> _children;
    std::map<unsigned int, GeometryNode*> _transparentChildren;
    
};

}
