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
  material->diffuse_texture = textureManager.createTexture("resources/textures/crate_diffuse.png", SRGBA);
  material->specular_texture = textureManager.createTexture("resources/textures/crate_specular.png", RGBA);
  material->reflection_map = textureManager.createTexture("resources/textures/specular.png", RGBA);
  material->shininess = 32.f;

  PointLight *pl = componentManager.createComponent<PointLight>(
      glm::vec3(0.2f, 0.2f, 0.2f),
      glm::vec3(0.6f, 0.6f, 0.6f),
      glm::vec3(0.6f, 0.6f, 0.6f));

  Transformation *t2 = componentManager.createComponent<Transformation>();

  t2->setRelativeTranslation(glm::vec3(0.f, 3.f, 0.f));
  t2->setRelativeRotation(glm::vec3(0.f, 45.f, 0.f));
  t2->setRelativeScaling(glm::vec3(1.f, 2.f, 1.f));
  Entity node2;
  node1.addChild(&node2);
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
  groundMat->diffuse_texture = textureManager.createTexture("resources/textures/wood.png", SRGBA);
  groundMat->shininess = 1.0f;
  Entity node4;
  root.addChild(&node4);
  Volume *ground = componentManager.createComponent<Volume>(Volume::createPlane(10.f, 10.f));
  node4.addComponent(ground);
  node4.addComponent(groundMat);

  Shader shader(
      "resources/shaders/basic.vs.glsl",
      "resources/shaders/basic.frag.glsl");

  // Render
  Engine engine;

  engine.setScene(&scene);
  engine.gameLoop();
}

void blinnPhong()
{
  ComponentManager componentManager;
  TextureManager textureManager;
  EntityManager entityManager;
  ModelLoader modelLoader(entityManager, componentManager, textureManager);

  DirectionLight *dl = componentManager.createComponent<DirectionLight>(
      glm::vec3(0.2f, 0.2f, 0.2f),
      glm::vec3(0.6f, 0.6f, 0.6f),
      glm::vec3(0.6f, 0.6f, 0.6f));

  Entity root;
  SceneGraph scene;
  CubeMap cubeMap("skybox", "resources/textures");
  scene.setCubeMap(&cubeMap);
  scene.setRoot(&root);

  root.addComponent(dl);

  Material *m1 = componentManager.createComponent<Material>();
  Volume *cube = componentManager.createComponent<Volume>(Volume::createCube(1.f));

  Entity node1;
  root.addChild(&node1);
  node1.addComponent(m1);
  node1.addComponent(cube);
  Transformation *t1 = componentManager.createComponent<Transformation>();
  node1.addComponent(t1);
  t1->rotate(glm::vec3(0.f, 45.f, 0.f));
  m1->diffuse_value = glm::vec3(0.8f, 0.4f, 0.2f);
  m1->specular_value = glm::vec3(1.f, 1.f, 1.f);
  m1->shininess = 0.5f;

  /*
  Entity node2;
  Material *m2 = componentManager.cloneComponent<Material>(m1);
  m2->shininess = 4.0f;
  root.addChild(&node2);
  node2.addComponent(m2);
  node2.addComponent(cube);
  Transformation *t2 = componentManager.cloneComponent<Transformation>(t1);
  node2.addComponent(t2);
  t2->translate(glm::vec3(2.f, 0.f, 0.f));
  


  Entity node3;
  Material *m3 = componentManager.cloneComponent<Material>(m1);
  m3->shininess = 8.0f;
  root.addChild(&node3);
  node3.addComponent(m3);
  node3.addComponent(cube);
  Transformation *t3 = componentManager.cloneComponent<Transformation>(t2);
  node3.addComponent(t3);
  t3->translate(glm::vec3(2.f, 0.f, 0.f));

  Entity node4;
  Material *m4 = componentManager.cloneComponent<Material>(m1);
  m4->shininess = 16.0f;
  root.addChild(&node4);
  node4.addComponent(m4);
  node4.addComponent(cube);
  Transformation *t4 = componentManager.cloneComponent<Transformation>(t3);
  node4.addComponent(t4);
  t4->translate(glm::vec3(2.f, 0.f, 0.f));

  Entity node5;
  Material *m5 = componentManager.cloneComponent<Material>(m1);
  m5->shininess = 32.0f;
  root.addChild(&node5);
  node5.addComponent(m5);
  node5.addComponent(cube);
  Transformation *t5 = componentManager.cloneComponent<Transformation>(t4);
  node5.addComponent(t5);
  t5->translate(glm::vec3(2.f, 0.f, 0.f));

  Entity node6;
  Material *m6 = componentManager.cloneComponent<Material>(m1);
  m6->shininess = 64.0f;
  root.addChild(&node6);
  node6.addComponent(m6);
  node6.addComponent(cube);
  Transformation *t6 = componentManager.cloneComponent<Transformation>(t5);
  node6.addComponent(t6);
  t6->translate(glm::vec3(2.f, 0.f, 0.f));

  Entity node7;
  Material *m7 = componentManager.cloneComponent<Material>(m1);
  m7->shininess = 128.0f;
  root.addChild(&node7);
  node7.addComponent(m7);
  node7.addComponent(cube);
  Transformation *t7 = componentManager.cloneComponent<Transformation>(t6);
  node7.addComponent(t7);
  t7->translate(glm::vec3(2.f, 0.f, 0.f));
  */

  Shader shader(
      "resources/shaders/basic.vs.glsl",
      "resources/shaders/basic.frag.glsl");

  Engine engine;

  engine.setScene(&scene);
  engine.gameLoop();
}

int main()
{
  cubeScene();
  //testInstanced();
  //blinnPhong();
  return 0;
}
