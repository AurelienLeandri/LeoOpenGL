
#pragma once

#include <renderer/global.hpp>
#include <SOIL.h>

#include <renderer/utils/shader.hpp>
#include <renderer/utils/framebuffer.hpp>
#include <renderer/scene/node.hpp>
#include <renderer/scene/alpha-node.hpp>
#include <renderer/scene/visitor/render-visitor.hpp>
#include <renderer/scene/model.hpp>
#include <renderer/scene/cube-map.hpp>
#include <renderer/renderer.hpp>


#include <model/components/types/drawable.hpp>

#include <vector>
#include <map>

namespace leo {

  using stringID = std::string;

  class InputManager;

class Engine {
  public:
    Engine();
    ~Engine();
    Engine(const Engine &other) = delete;
    Engine& operator=(const Engine &other) = delete;

  private:
    void _init();
    void doMovement(float deltaTime);

  public:
    void initRenderer(Shader shader);
    void setScene(model::Base *scene);
    void gameLoop();

  public: // Control attributes
    InputManager *inputManager;

  private:
    Camera *_camera;
    renderer::Renderer *_renderer;
    GLFWwindow *_window;
    model::Base *_scene;
    GLuint screenWidth = 800;
    GLuint screenHeight = 600;
};

}
