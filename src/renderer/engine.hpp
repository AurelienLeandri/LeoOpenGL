
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
#include <renderer/input-manager.hpp>
#include <renderer/camera.hpp>

#include <renderer/debug.hpp>

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
    CubeMap *_cubeMap;
    Node *_post_process_quad;
    RenderVisitor *render_visitor;
    RenderVisitor *render_visitor2;
    RenderVisitor *render_visitor3;
    RenderVisitor *post_process_render_visitor;
    GLFWwindow *_window;
    GLuint screenWidth = 800;
    GLuint screenHeight = 600;
};

}
