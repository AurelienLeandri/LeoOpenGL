#include <iostream>

#include <renderer/engine.hpp>
#include <renderer/utils/shader.hpp>
#include <model/components/material.hpp>
#include <model/components/drawable-collection.hpp>
#include <model/components/volume.hpp>
#include <model/base.hpp>

using namespace leo;

void print_matrix(const glm::mat4x4 &mat) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++)
      std::cout << mat[i][j] << " ";
    std::cout << std::endl;
  }
}

void cubeScene() {
  // Init scene
  model::Base scene;
  model::Material material;
  model::Volume cube = model::Volume::createCube(1.f);
  model::DrawableCollection drawables;
  drawables.addDrawable(&cube);
  material.diffuse_value = glm::vec3(0.89f, 0.42f, 0.11f);
  scene.addComponent("Material", &material);
  scene.addComponent("CubeVolume", &cube);
  scene.addComponent("Drawables", &drawables);

  Shader shader(
        "resources/shaders/default.vs.glsl",
        "resources/shaders/basic.frag.glsl"
        );

  // Render
  Engine engine;
  engine.initRenderer(shader);
  engine.setScene(&scene);
  engine.gameLoop();
}

int main()
{
  cubeScene();
  return 0;
}
