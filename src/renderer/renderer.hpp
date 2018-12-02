#pragma once

#include <renderer/global.hpp>
#include <renderer/utils/framebuffer.hpp>
#include <model/base.hpp>
#include <renderer/input-manager.hpp>
#include <renderer/camera.hpp>
#include <renderer/debug.hpp>

namespace leo {
  namespace renderer {

    class Renderer {
      public:
        Renderer();
        virtual ~Renderer();
        Renderer(const Renderer &other) = delete;

      public:
        const Renderer &operator=(const Renderer &other) = delete;
        Framebuffer &getOutput();
        void setWindowContext(GLFWwindow *window, InputManager *inputManager);
        void setCamera(Camera *camera);

      public:
        Framebuffer &render(model::Base *root);
        Framebuffer &render(model::Base *root,
            std::vector<const Framebuffer *> inputs);

      private:
        void _init();
        Framebuffer _output;
        Camera *_camera;
        GLFWwindow *_window;
        InputManager *_inputManager;
    };

  }  // namespace renderer
}  // namespace leo
