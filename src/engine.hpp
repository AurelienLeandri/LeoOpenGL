#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <material/material.hpp>
#include <material/basic-material.hpp>
#include <scene/node.hpp>
#include <scene/render-visitor.hpp>
#include <scene/cube-geometry.hpp>
#include <camera.hpp>

namespace leo {

class Engine{
  public:
    Engine();
    ~Engine();

  public:
    void gameLoop();

  private:
    void _init();
    void _updateCamera(float deltaTime, sf::Vector2f &formerPosition);
    Node *getRoot();

  private:
    Material *_material;
    Camera *_camera;
    Node *_root;
    sf::Window *_window;
};

}
