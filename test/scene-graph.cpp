#include <iostream>

#include <renderer/engine.hpp>
#include <renderer/shader.hpp>
#include <model/components/material.hpp>
#include <model/components/drawable-collection.hpp>
#include <model/components/volume.hpp>
#include <model/components/point-light.hpp>
#include <model/components/direction-light.hpp>
#include <model/components/transformation.hpp>
#include <model/entity.hpp>
#include <model/scene-graph.hpp>
#include <model/cube-map.hpp>
#include <model/model-loader.hpp>
#include <model/component-manager.hpp>
#include <model/texture-manager.hpp>
#include <model/entity-manager.hpp>

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
  model::ComponentManager componentManager;
  model::TextureManager textureManager;
  model::EntityManager entityManager;
  model::ModelLoader modelLoader(entityManager, componentManager, textureManager);

  model::Entity *m = modelLoader.loadModel("resources/models/nanosuit/nanosuit.obj");
  model::SceneGraph scene;
  model::CubeMap cubeMap("skybox", "resources/textures");
  scene.setCubeMap(&cubeMap);
  scene.setRoot(m);

  model::Entity node1;
  m->addChild(&node1);

  model::Material *material = componentManager.createComponent<model::Material>();
  std::cout << "Material type is " << material->getTypeId() << std::endl;

  model::Volume *cube = componentManager.createComponent<model::Volume>(model::Volume::createCube(1.f));
  std::cout << "Volume type is " << cube->getTypeId() << std::endl;

  model::DrawableCollection *drawables = componentManager.createComponent<model::DrawableCollection>();
  std::cout << "DrawableCollection type is " << drawables->getTypeId() << std::endl;

  drawables->addDrawable(cube);
  node1.addComponent(material);
  node1.addComponent(cube);
  node1.addComponent(drawables);
  material->diffuse_value = glm::vec3(0.89f, 0.42f, 0.11f);
  material->diffuse_texture = textureManager.createTexture("resources/textures/crate_diffuse.png");
  material->specular_texture = textureManager.createTexture("resources/textures/crate_specular.png");
  material->reflection_map = textureManager.createTexture("resources/textures/specular.png");

  model::Transformation *t2 = componentManager.createComponent<model::Transformation>();
  std::cout << "Transformation2 type is " << t2->getTypeId() << std::endl;

  model::PointLight *pl = componentManager.createComponent<model::PointLight>(
      glm::vec3(0.2f, 0.2f, 0.2f),
      glm::vec3(0.6f, 0.6f, 0.6f),
      glm::vec3(0.6f, 0.6f, 0.6f) );
  std::cout << "PointLight type is " << pl->getTypeId() << std::endl;
  t2->setRelativeTranslation(glm::vec3(3.f, 0.f, 0.f));
  t2->setRelativeRotation(glm::vec3(0.f, 45.f, 0.f));
  t2->setRelativeScaling(glm::vec3(1.f, 2.f, 1.f));
  model::Entity node2;
  node1.addChild(&node2);
  node2.addComponent(material);
  node2.addComponent(cube);
  node2.addComponent(drawables);
  node2.addComponent(t2);
  node2.addComponent(pl);

  model::Transformation *t3 = componentManager.createComponent<model::Transformation>();
  std::cout << "Transformation3 type is " << t3->getTypeId() << std::endl;

  model::DirectionLight *dl = componentManager.createComponent<model::DirectionLight>(
      glm::vec3(0.2f, 0.2f, 0.2f),
      glm::vec3(0.6f, 0.6f, 0.6f),
      glm::vec3(0.6f, 0.6f, 0.6f) );
  std::cout << "DirectionLight type is " << dl->getTypeId() << std::endl;

  t3->setRelativeTranslation(glm::vec3(4.f, 2.f, 0.f));
  t3->setRelativeRotation(glm::vec3(45.f, 0.f, 0.f));
  t3->setRelativeScaling(glm::vec3(0.5f, 0.5f, 0.5f));
  model::Entity node3;
  node1.addChild(&node3);
  node3.addComponent(material);
  node3.addComponent(cube);
  node3.addComponent(drawables);
  node3.addComponent(t3);
  node3.addComponent(dl);

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
