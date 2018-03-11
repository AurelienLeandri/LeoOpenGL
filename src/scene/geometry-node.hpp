#include "node.hpp"
#include "../material/material.hpp"
#include "../opengl-context.hpp"

class GeometryNode : public Node {
  public:
    ~GeometryNode();
    void setMaterial(Material *material);
    virtual void draw(const OpenGLContext &context) = 0;
  protected:
    Material *_material;
    std::vector<float> _data;
};
