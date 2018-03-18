#pragma once

#include "visitor.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace leo {

class TransformationVisitor : public Visitor {
  public:
    TransformationVisitor();
    TransformationVisitor(const TransformationVisitor &other);

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
