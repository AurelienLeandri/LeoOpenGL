#include <iostream>

#include <renderer/engine.hpp>
#include <renderer/utils/shader.hpp>
#include <model/components/material.hpp>
#include <model/components/drawable-collection.hpp>
#include <model/components/volume.hpp>
#include <model/base.hpp>
#include <model/scene-graph.hpp>

using namespace leo;

void print_matrix(const glm::mat4x4 &mat)
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
      std::cout << mat[i][j] << " ";
    std::cout << std::endl;
  }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void cubeScene()
{
  // Init scene
  model::Base root;
  model::Material material;
  model::Volume cube = model::Volume::createCube(1.f);
  model::DrawableCollection drawables;
  drawables.addDrawable(&cube);
  material.diffuse_value = glm::vec3(0.89f, 0.42f, 0.11f);
  root.addComponent("Material", &material);
  root.addComponent("CubeVolume", &cube);
  root.addComponent("Drawables", &drawables);

  Shader shader(
      "resources/shaders/basic.vs.glsl",
      "resources/shaders/basic.frag.glsl");

  // Render
  Engine engine;
  engine.initRenderer(shader);
  material.diffuse_texture = std::make_shared<Texture>("resources/textures/crate_diffuse.png");
  material.specular_texture = std::make_shared<Texture>("resources/textures/crate_specular.png");
  material.reflection_map = std::make_shared<Texture>("resources/textures/specular.png");
  model::SceneGraph scene;
  scene.setRoot(&root);
  engine.setScene(&scene);
  engine.gameLoop();
}

int main()
{
  cubeScene();
  return 0;
}
