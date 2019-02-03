
#pragma once

#include <renderer/global.hpp>

#include <renderer/scene/node.hpp>
#include <renderer/shader.hpp>

namespace leo {

class DrawableNode : public Node {
  public:
    DrawableNode();
    virtual ~DrawableNode();
    DrawableNode(const DrawableNode &other);

  public:
    virtual void draw(Shader *shader) = 0;

};

}
