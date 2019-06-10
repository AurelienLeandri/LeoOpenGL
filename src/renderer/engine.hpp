
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

public:
  void setRenderer(Renderer &renderer);
  void gameLoop();
  Camera &getCamera();
  GLFWwindow &getWindow();
  InputManager &getInputManager();

public: // Control attributes
  InputManager *inputManager;

private:
  Camera *_camera = nullptr;
  Renderer *_renderer = nullptr;
  GLFWwindow *_window = nullptr;
  GLuint screenWidth = 1620;
  GLuint screenHeight = 1080;
};

} // namespace leo
