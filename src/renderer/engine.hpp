
#pragma once

#include <renderer/global.hpp>
#include <SOIL.h>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/renderer.hpp>

#include <model/components/types/drawable.hpp>

#include <vector>
#include <map>

namespace leo
{

class InputManager;

namespace renderer
{

using stringID = std::string;

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
