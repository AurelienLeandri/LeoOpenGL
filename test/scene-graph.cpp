#include <iostream>

#include <renderer/renderer.hpp>
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
  model::Base b;
  model::Material material;
  model::Volume cube = model::Volume::createCube(1.f);
  model::DrawableCollection drawables;
  drawables.addDrawable(&cube);
  material.diffuse_value = glm::vec3(0.89f, 0.42f, 0.11f);
  b.addComponent("Material", &material);
  b.addComponent("CubeVolume", &cube);
  b.addComponent("Drawables", &drawables);

  // Init renderer
  renderer::Renderer renderer(Shader(
        "resources/shaders/default.vs.glsl",
        "resources/shaders/basic.frag.glsl"
        ));

  // Render
  renderer.render(&b);
}

int main()
{
  return 0;
}
