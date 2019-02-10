
#pragma once

#include <renderer/global.hpp>

#include <SOIL.h>

#include <vector>
#include <map>

namespace leo
{

class InputManager;

namespace model {
  class SceneGraph;
}

namespace renderer
{

class Shader;
class Renderer;
class InputManager;
class Camera;

using t_id = std::string;

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
  void initRenderer(Shader shader);
  void setScene(model::SceneGraph *scene);
  void gameLoop();

public: // Control attributes
  InputManager *inputManager;

private:
  Camera *_camera = nullptr;
  renderer::Renderer *_renderer = nullptr;
  GLFWwindow *_window = nullptr;
  model::SceneGraph *_scene = nullptr;
  GLuint screenWidth = 1620;
  GLuint screenHeight = 1080;
};

} // namespace renderer

} // namespace leo
