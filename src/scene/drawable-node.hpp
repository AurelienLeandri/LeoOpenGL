
#pragma once

#include <global.hpp>

#include <scene/node.hpp>
#include <utils/shader.hpp>

namespace leo {

class DrawableNode : public Node {
  public:
    DrawableNode();
    virtual ~DrawableNode();
    DrawableNode(const DrawableNode &other);
    DrawableNode &operator=(const DrawableNode &other);

  public:
    virtual void draw(Shader *shader) = 0;

};

}
