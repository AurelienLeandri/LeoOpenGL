#pragma once

#include <renderer/global.hpp>
#include <renderer/utils/framebuffer.hpp>
#include <model/base.hpp>
#include <renderer/input-manager.hpp>
#include <renderer/camera.hpp>
#include <renderer/debug.hpp>
#include <renderer/utils/shader.hpp>

#include <model/components/drawable-collection.hpp>
#include <model/components/volume.hpp>
#include <model/components/transformation.hpp>


namespace leo {

  namespace model {
    class Material;
  }

  namespace renderer {

    class Renderer {
      public:
        Renderer(GLFWwindow *window,
            InputManager *inputManager,
            Camera *camera,
            Shader shader
            );
        virtual ~Renderer();
        Renderer(const Renderer &other) = delete;

      public:
        const Renderer &operator=(const Renderer &other) = delete;
        Framebuffer &getOutput();

      public:
        Framebuffer &render(model::Base *root);
        Framebuffer &render(model::Base *root,
            std::vector<const Framebuffer *> inputs);

      private:
        void _renderRec(model::Base *root,
            std::vector<const Framebuffer *> inputs);
        void _setCurrentMaterial(model::Material *material);
        void _setModelMatrix(model::Transformation *transformation);
        void _setWindowContext(GLFWwindow *window, InputManager *inputManager);
        void _setCamera(Camera *camera);
        void _drawCollection(model::DrawableCollection *collection);

      private:
        void _init();
        Framebuffer _output;
        Camera *_camera;
        GLFWwindow *_window;
        InputManager *_inputManager;
        Shader _shader;
    };

  }  // namespace renderer
}  // namespace leo
