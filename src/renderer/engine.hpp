
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
class SceneGraphObserver;

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
  void _reloadScene();

public:
  void initRenderer(Shader shader);
  void setScene(SceneGraph *scene);
  void gameLoop();

public: // Control attributes
  InputManager *inputManager;

private:
  Camera *_camera = nullptr;
  Renderer *_renderer = nullptr;
  GLFWwindow *_window = nullptr;
  SceneGraph *_scene = nullptr;
  SceneGraphObserver *_observer = nullptr;
  GLuint screenWidth = 1620;
  GLuint screenHeight = 1080;
};

} // namespace leo
