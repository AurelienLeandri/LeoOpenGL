
#pragma once

#include <global.hpp>
#include <SOIL.h>

#include <utils/shader.hpp>
#include <scene/node.hpp>
#include <scene/visitor/render-visitor.hpp>
#include <scene/model.hpp>
#include <input-manager.hpp>
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
    Node *getRoot();

  private: // Control methods and callbacks
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
    void doMovement(float deltaTime);

  public: // Control attributes
    InputManager inputManager;

  private:
    Camera *_camera;
    Node *_root;
    RenderVisitor *render_visitor;
    GLFWwindow *_window;
    GLuint screenWidth = 800;
    GLuint screenHeight = 600;
};

}
