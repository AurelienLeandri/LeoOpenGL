#pragma once

#include "opengl-context.hpp"
#include "material/material.hpp"

class Engine {
  public:
    Engine();
    ~Engine();

  public:
    void gameLoop();

  private:
    void _init();
    void _updateCamera(Camera &camera, float deltaTime, sf::Vector2f &formerPosition);

  private:
    OpenGLContext *_context;
    Material *_material;
    Camera *_camera;
};

