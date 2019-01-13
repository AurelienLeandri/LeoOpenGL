#pragma once

#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <renderer/scene/visitor/visitor.hpp>
#include <renderer/scene/geometry-node.hpp>

namespace leo {

class TransformationVisitor : public Visitor {
  public:
    TransformationVisitor();
    virtual ~TransformationVisitor();
    TransformationVisitor(const TransformationVisitor &other);
    TransformationVisitor &operator=(const TransformationVisitor &other);

  public:
    virtual void visit(Node *node);

  public:
    void setTransformation(glm::mat4x4 transformation);
    void translate(glm::vec3 v);
    void rotate(float deg, glm::vec3 v);
    void scale(glm::vec3 v);
    void transform(glm::mat4x4 transformation);

  protected:
    glm::mat4x4 _transformation;
};

}
