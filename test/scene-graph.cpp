#include <iostream>

#include <renderer/engine.hpp>
#include <renderer/shader.hpp>
#include <model/components/material.hpp>
#include <model/components/volume.hpp>
#include <model/components/point-light.hpp>
#include <model/components/direction-light.hpp>
#include <model/components/transformation.hpp>
#include <model/components/instanced.hpp>
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

void testInstanced()
{
  ComponentManager componentManager;
  TextureManager textureManager;
  EntityManager entityManager;
  ModelLoader modelLoader(entityManager, componentManager, textureManager);

  Entity *planet = modelLoader.loadModel("resources/models/planet", "planet.obj");
  Entity *rock = modelLoader.loadModel("resources/models/rock", "rock.obj");

  SceneGraph scene;
  CubeMap cubeMap("skybox", "resources/textures");
  scene.setCubeMap(&cubeMap);
  scene.setRoot(planet);

  SceneGraph instancedScene;
  instancedScene.setRoot(rock);

  DirectionLight *dl = componentManager.createComponent<DirectionLight>(
      glm::vec3(0.2f, 0.2f, 0.2f),
      glm::vec3(0.6f, 0.6f, 0.6f),
      glm::vec3(0.6f, 0.6f, 0.6f));
  planet->addComponent(dl);
  rock->addComponent(dl);

  unsigned int amount = 1000;
  srand(glfwGetTime()); // initialize random seed
  float radius = 50.0;
  float offset = 2.5f;
  std::vector<glm::mat4> transformations;
  for (unsigned int i = 0; i < amount; i++)
  {
    glm::mat4 model = glm::mat4(1.0f);
    // 1. translation: displace along circle with 'radius' in range [-offset, offset]
    float angle = (float)i / (float)amount * 360.0f;
    float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float x = sin(angle) * radius + displacement;
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float z = cos(angle) * radius + displacement;
    model = glm::translate(model, glm::vec3(x, y, z));

    // 2. scale: Scale between 0.05 and 0.25f
    float scale = (rand() % 20) / 100.0f + 0.05;
    model = glm::scale(model, glm::vec3(scale));

    // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
    float rotAngle = (rand() % 360);
    model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

    // 4. now add to list of matrices
    transformations.push_back(model);
  }

  Shader shader(
      "resources/shaders/basic.vs.glsl",
      "resources/shaders/basic.frag.glsl");

  // Render
  Engine engine;
  engine.initRenderer(shader);

  engine.setScene(&scene);
  engine.setInstancedScene(&instancedScene, transformations);
  engine.gameLoop();
}

void cubeScene()
{
  ComponentManager componentManager;
  TextureManager textureManager;
  EntityManager entityManager;
  ModelLoader modelLoader(entityManager, componentManager, textureManager);

  Entity root;

  Entity *m = modelLoader.loadModel("resources/models/nanosuit/", "nanosuit.obj");
  root.addChild(m);
  SceneGraph scene;
  CubeMap cubeMap("skybox", "resources/textures");
  scene.setCubeMap(&cubeMap);
  scene.setRoot(&root);

  Entity node1;
  root.addChild(&node1);

  Material *material = componentManager.createComponent<Material>();

  Volume *cube = componentManager.createComponent<Volume>(Volume::createCube(1.f));

  node1.addComponent(material);
  node1.addComponent(cube);
  material->diffuse_value = glm::vec3(0.89f, 0.42f, 0.11f);
  material->diffuse_texture = textureManager.createTexture("resources/textures/crate_diffuse.png");
  material->specular_texture = textureManager.createTexture("resources/textures/crate_specular.png");
  material->reflection_map = textureManager.createTexture("resources/textures/specular.png");

  PointLight *pl = componentManager.createComponent<PointLight>(
      glm::vec3(0.2f, 0.2f, 0.2f),
      glm::vec3(0.6f, 0.6f, 0.6f),
      glm::vec3(0.6f, 0.6f, 0.6f));

  Transformation *t2 = componentManager.createComponent<Transformation>();

  t2->setRelativeTranslation(glm::vec3(3.f, 0.f, 0.f));
  t2->setRelativeRotation(glm::vec3(0.f, 45.f, 0.f));
  t2->setRelativeScaling(glm::vec3(1.f, 2.f, 1.f));
  Entity node2;
  node1.addChild(&node2);
  node2.addComponent(cube);
  node2.addComponent(t2);
  node2.addComponent(pl);

  Transformation *t3 = componentManager.createComponent<Transformation>();

  DirectionLight *dl = componentManager.createComponent<DirectionLight>(
      glm::vec3(0.2f, 0.2f, 0.2f),
      glm::vec3(0.6f, 0.6f, 0.6f),
      glm::vec3(0.6f, 0.6f, 0.6f));

  t3->setRelativeTranslation(glm::vec3(4.f, 2.f, 0.f));
  t3->setRelativeRotation(glm::vec3(45.f, 0.f, 0.f));
  t3->setRelativeScaling(glm::vec3(0.5f, 0.5f, 0.5f));
  Entity node3;
  node1.addChild(&node3);
  node3.addComponent(cube);
  node3.addComponent(t3);
  node3.addComponent(dl);

  Material *groundMat = componentManager.createComponent<Material>();
  groundMat->diffuse_texture = textureManager.createTexture("resources/textures/wood.png");
  Entity node4;
  root.addChild(&node4);
  Volume *ground = componentManager.createComponent<Volume>(Volume::createPlane(10.f, 10.f));
  node4.addComponent(ground);
  node4.addComponent(groundMat);
  //Transformation *t4 = componentManager.createComponent<Transformation>();
  //t4->rotate()

  Shader shader(
      "resources/shaders/basic.vs.glsl",
      "resources/shaders/basic.frag.glsl");

  // Render
  Engine engine;
  engine.initRenderer(shader);

  engine.setScene(&scene);
  engine.gameLoop();
}

int main()
{
  cubeScene();
  //testInstanced();
  return 0;
}
