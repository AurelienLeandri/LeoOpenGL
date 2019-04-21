
#pragma once

#include <renderer/global.hpp>

#include <SOIL.h>

#include <vector>
#include <map>

namespace leo
{

class InputManager;
class SceneGraph;
class Shader;
class Renderer;
class InputManager;
class Camera;

class Engine
{
public:
  Engine();
  ~Engine();
  Engine(const Engine &other) = delete;
  Engine &operator=(const Engine &other) = delete;

private:
  void _init();
  void doMovement(float deltaTime);
  void _initRenderer(Shader shader);

public:
  void setScene(SceneGraph *scene);
  void setInstancedScene(SceneGraph *instancedScene, const std::vector<glm::mat4> &transformations);
  void gameLoop();

public: // Control attributes
  InputManager *inputManager;

private:
  Camera *_camera = nullptr;
  Renderer *_renderer = nullptr;
  GLFWwindow *_window = nullptr;
  SceneGraph *_scene = nullptr;
  GLuint screenWidth = 1620;
  GLuint screenHeight = 1080;
};

} // namespace leo
