#include <iostream>

#include <renderer/engine.hpp>
#include <renderer/shader.hpp>
#include <model/components/material.hpp>
#include <model/components/drawable-collection.hpp>
#include <model/components/volume.hpp>
#include <model/components/point-light.hpp>
#include <model/components/direction-light.hpp>
#include <model/components/transformation.hpp>
#include <model/base.hpp>
#include <model/scene-graph.hpp>
#include <model/cube-map.hpp>
#include <model/model-loader.hpp>

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
  model::Base *m = model::ModelLoader::loadModel("resources/models/nanosuit/nanosuit.obj");
  model::SceneGraph scene;
  model::CubeMap cubeMap("skybox", "resources/textures");
  scene.setCubeMap(&cubeMap);
  scene.setRoot(m);

  model::Base node1;
  m->addChild(&node1);

  model::Material material;
  model::Volume cube = model::Volume::createCube(1.f);
  model::DrawableCollection drawables;
  drawables.addDrawable(&cube);
  node1.addComponent("Material", &material);
  node1.addComponent("CubeVolume", &cube);
  node1.addComponent("Drawables", &drawables);
  material.diffuse_value = glm::vec3(0.89f, 0.42f, 0.11f);
  material.diffuse_texture = std::make_shared<Texture>("resources/textures/crate_diffuse.png");
  material.specular_texture = std::make_shared<Texture>("resources/textures/crate_specular.png");
  material.reflection_map = std::make_shared<Texture>("resources/textures/specular.png");

  model::Transformation t2;
  model::PointLight pl(
      glm::vec3(0.2f, 0.2f, 0.2f),
      glm::vec3(0.6f, 0.6f, 0.6f),
      glm::vec3(0.6f, 0.6f, 0.6f) );
  t2.setRelativeTranslation(glm::vec3(3.f, 0.f, 0.f));
  t2.setRelativeRotation(glm::vec3(0.f, 45.f, 0.f));
  t2.setRelativeScaling(glm::vec3(1.f, 2.f, 1.f));
  model::Base node2;
  node1.addChild(&node2);
  node2.addComponent("Material", &material);
  node2.addComponent("CubeVolume", &cube);
  node2.addComponent("Drawables", &drawables);
  node2.addComponent("Transformation", &t2);
  node2.addComponent("PointLight", &pl);

  model::Transformation t3;
  model::DirectionLight dl(
      glm::vec3(0.2f, 0.2f, 0.2f),
      glm::vec3(0.6f, 0.6f, 0.6f),
      glm::vec3(0.6f, 0.6f, 0.6f) );
  t3.setRelativeTranslation(glm::vec3(4.f, 2.f, 0.f));
  t3.setRelativeRotation(glm::vec3(45.f, 0.f, 0.f));
  t3.setRelativeScaling(glm::vec3(0.5f, 0.5f, 0.5f));
  model::Base node3;
  node1.addChild(&node3);
  node3.addComponent("Material", &material);
  node3.addComponent("CubeVolume", &cube);
  node3.addComponent("Drawables", &drawables);
  node3.addComponent("Transformation", &t3);
  node3.addComponent("DirectionLight", &dl);

  renderer::Shader shader(
      "resources/shaders/basic.vs.glsl",
      "resources/shaders/basic.frag.glsl");

  // Render
  renderer::Engine engine;
  engine.initRenderer(shader);

  engine.setScene(&scene);
  engine.gameLoop();
}

int main()
{
  cubeScene();
  return 0;
}
