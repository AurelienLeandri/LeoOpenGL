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
  engine.gameLoop();
  return 0;
}
