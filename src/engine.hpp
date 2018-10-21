
#pragma once

#include <global.hpp>
#include <SOIL.h>

#include <utils/shader.hpp>
#include <utils/framebuffer.hpp>
#include <scene/node.hpp>
#include <scene/alpha-node.hpp>
#include <scene/visitor/render-visitor.hpp>
#include <scene/model.hpp>
#include <input-manager.hpp>
#include <camera.hpp>

namespace leo {

class Engine{
  public:
    Engine();
    ~Engine();
    Engine(const Engine &other) = delete;
    Engine& operator=(const Engine &other) = delete;

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
    Node *_root2;
    Node *_post_process_quad;
    RenderVisitor *render_visitor;
    RenderVisitor *render_visitor2;
    RenderVisitor *post_process_render_visitor;
    GLFWwindow *_window;
    GLuint screenWidth = 800;
    GLuint screenHeight = 600;
};

}
