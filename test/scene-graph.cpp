#include <iostream>

#include <renderer/engine.hpp>
#include <renderer/shader.hpp>
#include <renderer/renderer.hpp>
#include <renderer/opengl-context.hpp>
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

/*
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

  node1.addComponent(cube);
  material->diffuse_texture = textureManager.createTexture("resources/textures/wood.png", RGBA);
  material->specular_texture = textureManager.createTexture("resources/textures/wood.png", RGBA);
  material->normal_map = textureManager.createTexture("resources/textures/toy_box_normal.png", RGBA);
  material->parallax_map = textureManager.createTexture("resources/textures/toy_box_disp.png", RGBA);
  material->shininess = 32.f;

  Material *material2 = componentManager.createComponent<Material>();
  material2->diffuse_texture = textureManager.createTexture("resources/textures/bricks2.jpg", RGB);
  material2->specular_texture = textureManager.createTexture("resources/textures/bricks2.jpg", RGB);
  material2->normal_map = textureManager.createTexture("resources/textures/bricks2_normal.jpg", RGB);
  material2->parallax_map = textureManager.createTexture("resources/textures/bricks2_disp.jpg", RGB);
  material2->shininess = 32.f;

  node1.addComponent(material2);

  PointLight *pl = componentManager.createComponent<PointLight>(
      glm::vec3(9.2f, 9.2f, 9.2f),
      glm::vec3(9.6f, 9.6f, 9.6f),
      glm::vec3(9.6f, 9.6f, 9.6f));

  Transformation *t2 = componentManager.createComponent<Transformation>();

  t2->setRelativeTranslation(glm::vec3(0.f, 2.f, 0.f));
  t2->setRelativeRotation(glm::vec3(0.f, 45.f, 0.f));
  t2->setRelativeScaling(glm::vec3(1.f, 2.f, 1.f));
  Entity node2;
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
  node3.addComponent(cube);
  node3.addComponent(t3);
  node3.addComponent(dl);
  node3.addComponent(material);

  Material *groundMaterial = componentManager.createComponent<Material>();
  groundMaterial->diffuse_texture = textureManager.createTexture("resources/textures/brickwall.jpg", SRGB);
  groundMaterial->specular_texture = textureManager.createTexture("resources/textures/brickwall.jpg", SRGB);
  groundMaterial->normal_map = textureManager.createTexture("resources/textures/brickwall_normal.jpg", RGB);
  groundMaterial->shininess = 32.f;

  Material *wallMat = componentManager.createComponent<Material>();
  wallMat->diffuse_texture = textureManager.createTexture("resources/textures/wood.png", SRGBA);
  wallMat->specular_texture = wallMat->diffuse_texture;
  wallMat->shininess = 1.0f;

  Entity node4;
  root.addChild(&node4);
  Volume *ground = componentManager.createComponent<Volume>(Volume::createPlane(10.f, 10.f));
  node4.addComponent(groundMaterial);

  Entity node40;
  node4.addChild(&node40);
  node40.addComponent(ground);
  node40.addComponent(wallMat);

  Entity node41;
  Transformation *t41 = componentManager.createComponent<Transformation>();
  t41->setRelativeScaling(glm::vec3(1.f, 1.f, 2.f));
  t41->setRelativeRotation(glm::vec3(3.14 / 2.0, 0.f, 0.f));
  t41->setRelativeTranslation(glm::vec3(0.f, 10.f, -5.f));
  node4.addChild(&node41);
  node41.addComponent(ground);
  node41.addComponent(t41);

  Entity node42;
  Transformation *t42 = componentManager.createComponent<Transformation>();
  t42->setRelativeScaling(glm::vec3(1.f, 1.f, 2.f));
  t42->setRelativeRotation(glm::vec3(3.14 / 2.0, 0.f, 3.14f));
  t42->setRelativeTranslation(glm::vec3(0.f, 10.f, 5.f));
  node4.addChild(&node42);
  node42.addComponent(ground);
  node42.addComponent(t42);

  Entity node43;
  Transformation *t43 = componentManager.createComponent<Transformation>();
  t43->setRelativeScaling(glm::vec3(1.f, 1.f, 2.f));
  t43->setRelativeRotation(glm::vec3(3.14 / 2.0, 0.f, 3.14f / 2.f));
  t43->setRelativeTranslation(glm::vec3(5.f, 10.f, 0.f));
  node4.addChild(&node43);
  node43.addComponent(ground);
  node43.addComponent(t43);

  Entity node44;
  Transformation *t44 = componentManager.createComponent<Transformation>();
  t44->setRelativeScaling(glm::vec3(1.f, 1.f, 2.f));
  t44->setRelativeRotation(glm::vec3(3.14 / 2.0, 0.f, -3.14f / 2.f));
  t44->setRelativeTranslation(glm::vec3(-5.f, 10.f, 0.f));
  node4.addChild(&node44);
  node44.addComponent(ground);
  node44.addComponent(t44);

  Entity node45;
  Transformation *t45 = componentManager.createComponent<Transformation>();
  t45->setRelativeRotation(glm::vec3(0.f, 0.f, 3.14f));
  t45->setRelativeTranslation(glm::vec3(0.f, 20.f, 0.f));
  node4.addChild(&node45);
  node45.addComponent(ground);
  node45.addComponent(t45);

  Shader shader(
      "resources/shaders/basic.vs.glsl",
      "resources/shaders/basic.frag.glsl");

  // Render
  Engine engine;

  engine.setScene(&scene);

  // Test oberver mode
  node1.addChild(&node3);
  node1.addChild(&node2);

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

  Shader shader(
      "resources/shaders/basic.vs.glsl",
      "resources/shaders/basic.frag.glsl");

  Engine engine;

  engine.setScene(&scene);
  engine.gameLoop();
}
*/





void decoupling()
{
  // Scene model
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

  node1.addComponent(cube);
  material->diffuse_texture = textureManager.createTexture("resources/textures/wood.png", RGBA);
  material->specular_texture = textureManager.createTexture("resources/textures/wood.png", RGBA);
  material->normal_map = textureManager.createTexture("resources/textures/toy_box_normal.png", RGBA);
  material->parallax_map = textureManager.createTexture("resources/textures/toy_box_disp.png", RGBA);
  material->shininess = 32.f;

  Material *material2 = componentManager.createComponent<Material>();
  material2->diffuse_texture = textureManager.createTexture("resources/textures/bricks2.jpg", RGB);
  material2->specular_texture = textureManager.createTexture("resources/textures/bricks2.jpg", RGB);
  material2->normal_map = textureManager.createTexture("resources/textures/bricks2_normal.jpg", RGB);
  material2->parallax_map = textureManager.createTexture("resources/textures/bricks2_disp.jpg", RGB);
  material2->shininess = 32.f;

  node1.addComponent(material2);

  PointLight *pl = componentManager.createComponent<PointLight>(
      glm::vec3(9.2f, 9.2f, 9.2f),
      glm::vec3(9.6f, 9.6f, 9.6f),
      glm::vec3(9.6f, 9.6f, 9.6f));

  Transformation *t2 = componentManager.createComponent<Transformation>();

  t2->setRelativeTranslation(glm::vec3(0.f, 2.f, 0.f));
  t2->setRelativeRotation(glm::vec3(0.f, 45.f, 0.f));
  t2->setRelativeScaling(glm::vec3(1.f, 2.f, 1.f));
  Entity node2;
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
  node3.addComponent(cube);
  node3.addComponent(t3);
  node3.addComponent(dl);
  node3.addComponent(material);

  Material *groundMaterial = componentManager.createComponent<Material>();
  groundMaterial->diffuse_texture = textureManager.createTexture("resources/textures/brickwall.jpg", SRGB);
  groundMaterial->specular_texture = textureManager.createTexture("resources/textures/brickwall.jpg", SRGB);
  groundMaterial->normal_map = textureManager.createTexture("resources/textures/brickwall_normal.jpg", RGB);
  groundMaterial->shininess = 32.f;

  Material *wallMat = componentManager.createComponent<Material>();
  wallMat->diffuse_texture = textureManager.createTexture("resources/textures/wood.png", SRGBA);
  wallMat->specular_texture = wallMat->diffuse_texture;
  wallMat->shininess = 1.0f;

  Entity node4;
  root.addChild(&node4);
  Volume *ground = componentManager.createComponent<Volume>(Volume::createPlane(10.f, 10.f));
  node4.addComponent(groundMaterial);

  Entity node40;
  node4.addChild(&node40);
  node40.addComponent(ground);
  node40.addComponent(wallMat);

  Entity node41;
  Transformation *t41 = componentManager.createComponent<Transformation>();
  t41->setRelativeScaling(glm::vec3(1.f, 1.f, 2.f));
  t41->setRelativeRotation(glm::vec3(3.14 / 2.0, 0.f, 0.f));
  t41->setRelativeTranslation(glm::vec3(0.f, 10.f, -5.f));
  node4.addChild(&node41);
  node41.addComponent(ground);
  node41.addComponent(t41);

  Entity node42;
  Transformation *t42 = componentManager.createComponent<Transformation>();
  t42->setRelativeScaling(glm::vec3(1.f, 1.f, 2.f));
  t42->setRelativeRotation(glm::vec3(3.14 / 2.0, 0.f, 3.14f));
  t42->setRelativeTranslation(glm::vec3(0.f, 10.f, 5.f));
  node4.addChild(&node42);
  node42.addComponent(ground);
  node42.addComponent(t42);

  Entity node43;
  Transformation *t43 = componentManager.createComponent<Transformation>();
  t43->setRelativeScaling(glm::vec3(1.f, 1.f, 2.f));
  t43->setRelativeRotation(glm::vec3(3.14 / 2.0, 0.f, 3.14f / 2.f));
  t43->setRelativeTranslation(glm::vec3(5.f, 10.f, 0.f));
  node4.addChild(&node43);
  node43.addComponent(ground);
  node43.addComponent(t43);

  Entity node44;
  Transformation *t44 = componentManager.createComponent<Transformation>();
  t44->setRelativeScaling(glm::vec3(1.f, 1.f, 2.f));
  t44->setRelativeRotation(glm::vec3(3.14 / 2.0, 0.f, -3.14f / 2.f));
  t44->setRelativeTranslation(glm::vec3(-5.f, 10.f, 0.f));
  node4.addChild(&node44);
  node44.addComponent(ground);
  node44.addComponent(t44);

  Entity node45;
  Transformation *t45 = componentManager.createComponent<Transformation>();
  t45->setRelativeRotation(glm::vec3(0.f, 0.f, 3.14f));
  t45->setRelativeTranslation(glm::vec3(0.f, 20.f, 0.f));
  node4.addChild(&node45);
  node45.addComponent(ground);
  node45.addComponent(t45);


  // Engine creating camera, windows and input manager (do Getters)
  Engine engine;
  Camera &camera = engine.getCamera();
  GLFWwindow &window = engine.getWindow();
  InputManager &inputManager = engine.getInputManager();

  // OpenGLContext(+ SceneData inside)
  OpenGLContext context;
  context.setWindowContext(window, inputManager);

  // Renderer
  Renderer renderer;
  renderer.createSceneContext(context);
  renderer.setSceneGraph(scene);

  // Create nodes
  
  // Engine->SetRenderer
  engine.setRenderer(renderer);
  engine.gameLoop();
  // Engine->GameLoop
}

int main()
{
  decoupling();
  //cubeScene();
  //testInstanced();
  //blinnPhong();
  return 0;
}
