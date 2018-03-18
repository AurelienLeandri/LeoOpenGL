#include "node.hpp"
#include "../material/material.hpp"
#include "../opengl-context.hpp"

class GeometryNode : public Node {
  public:
    ~GeometryNode();
    virtual void draw(const OpenGLContext &context) = 0;

  public:
    const glm::mat4x4 &getModelMatrix() const;
    void setMaterial(Material *material);
    void transform(const glm::mat4x4 &transformation);

  public:
    const glm::mat4x4 &getModelMatrix();

  protected:
    Material *_material;
    std::vector<float> _data;
    glm::mat4x4 _modelMatrix;
};
