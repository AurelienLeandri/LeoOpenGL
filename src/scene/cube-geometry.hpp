#include "geometry-node.hpp"

class CubeGeometry : public GeometryNode {
  public:
    virtual void draw(const OpenGLContext &context);
    virtual void update(double delta);
};
