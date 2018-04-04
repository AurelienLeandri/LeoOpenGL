#include <iostream>

#include <engine.hpp>
#include <scene/cube-geometry.hpp>
#include <scene/transformation-visitor.hpp>

void print_matrix(const glm::mat4x4 &mat) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++)
      std::cout << mat[i][j] << " ";
    std::cout << std::endl;
  }
}

int main()
{
  leo::Engine engine;
  leo::CubeGeometry *root = new leo::CubeGeometry();
  root->addChild(new leo::CubeGeometry());
  root->addChild(new leo::CubeGeometry());
  root->addChild(new leo::CubeGeometry());
  for (auto *e : root->getChildren()) {
    e->addChild(new leo::CubeGeometry());
    e->addChild(new leo::CubeGeometry());
  }
  leo::TransformationVisitor transformationVisitor;
  transformationVisitor.translate(glm::vec3(0.0, 0.0, 2.0));
  transformationVisitor.visit(root);
  transformationVisitor.translate(glm::vec3(0.0, -1.0, -2.0));
  transformationVisitor.visit(root->getChildren()[0]);
  print_matrix(dynamic_cast<leo::GeometryNode*>(root)->getModelMatrix());
  print_matrix(dynamic_cast<leo::GeometryNode*>(root->getChildren()[0])->getModelMatrix());
  print_matrix(dynamic_cast<leo::GeometryNode*>(root->getChildren()[0]->getChildren()[0])->getModelMatrix());
  print_matrix(dynamic_cast<leo::GeometryNode*>(root->getChildren()[1]->getChildren()[0])->getModelMatrix());
  delete root;
  return 0;
}
