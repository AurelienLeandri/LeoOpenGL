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

  model::Volume *cube = componentManager.createComponent<model::Volume>(model::Volume::createCube(1.f));

  node1.addComponent(material);
  node1.addComponent(cube);
  material->diffuse_value = glm::vec3(0.89f, 0.42f, 0.11f);
  material->diffuse_texture = textureManager.createTexture("resources/textures/crate_diffuse.png");
  material->specular_texture = textureManager.createTexture("resources/textures/crate_specular.png");
  material->reflection_map = textureManager.createTexture("resources/textures/specular.png");

  model::PointLight *pl = componentManager.createComponent<model::PointLight>(
      glm::vec3(0.2f, 0.2f, 0.2f),
      glm::vec3(0.6f, 0.6f, 0.6f),
      glm::vec3(0.6f, 0.6f, 0.6f));

  model::Transformation *t2 = componentManager.createComponent<model::Transformation>();

  t2->setRelativeTranslation(glm::vec3(3.f, 0.f, 0.f));
  t2->setRelativeRotation(glm::vec3(0.f, 45.f, 0.f));
  t2->setRelativeScaling(glm::vec3(1.f, 2.f, 1.f));
  model::Entity node2;
  node1.addChild(&node2);
  node2.addComponent(material);
  node2.addComponent(cube);
  node2.addComponent(t2);
  node2.addComponent(pl);

  model::Transformation *t3 = componentManager.createComponent<model::Transformation>();

  model::DirectionLight *dl = componentManager.createComponent<model::DirectionLight>(
      glm::vec3(0.2f, 0.2f, 0.2f),
      glm::vec3(0.6f, 0.6f, 0.6f),
      glm::vec3(0.6f, 0.6f, 0.6f));

  t3->setRelativeTranslation(glm::vec3(4.f, 2.f, 0.f));
  t3->setRelativeRotation(glm::vec3(45.f, 0.f, 0.f));
  t3->setRelativeScaling(glm::vec3(0.5f, 0.5f, 0.5f));
  model::Entity node3;
  node1.addChild(&node3);
  node3.addComponent(material);
  node3.addComponent(cube);
  node3.addComponent(t3);
  node3.addComponent(dl);

  // TEST HARDWARE INSTANCING
  model::Entity node4;
  model::Instanced *instanced = componentManager.createComponent<model::Instanced>(model::Volume::createCube(1.f));
  unsigned int amount = 1000;
  srand(glfwGetTime()); // initialize random seed
  float radius = 50.0;
  float offset = 2.5f;
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
    instanced->transformations.push_back(model);
  }
  node3.addChild(&node4);
  node4.addComponent(instanced);

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
