#pragma once

#include <renderer/global.hpp>
#include <renderer/utils/framebuffer.hpp>
#include <model/base.hpp>
#include <renderer/input-manager.hpp>
#include <renderer/camera.hpp>
#include <renderer/debug.hpp>

#include <model/components/drawable-collection.hpp>
#include <model/components/volume.hpp>
#include <model/components/transformation.hpp>

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
        Framebuffer &_renderRec(model::Base *root,
            std::vector<const Framebuffer *> inputs);
        //void _setCurrentMaterial(model::Material *material);
        void _setModelMatrix(model::Transformation *transformation);

      private:
        void _init();
        Framebuffer _output;
        Camera *_camera;
        GLFWwindow *_window;
        InputManager *_inputManager;
    };

  }  // namespace renderer
}  // namespace leo
