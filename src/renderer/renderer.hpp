#pragma once

#include <renderer/global.hpp>
#include <renderer/light-uniforms.hpp>
#include <renderer/texture-wrapper.hpp>
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

    class DirectionLightUniform;
    class PointLightUniform;

      typedef struct BufferCollection
      {
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
    } BufferCollection;

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
        Framebuffer &render(const model::SceneGraph *sceneGraph);
        Framebuffer &render(const model::SceneGraph *sceneGraph,
            std::vector<const Framebuffer *> inputs);

      private:
        void _renderRec(const model::Base *root,
            std::vector<const Framebuffer *> inputs);
        void _setCurrentMaterial(model::Material *material);
        void _setModelMatrix(model::Transformation *transformation);
        void _setModelMatrix();
        void _setWindowContext(GLFWwindow *window, InputManager *inputManager);
        void _setCamera(Camera *camera);
        void _drawCollection(model::DrawableCollection *collection);
        void _drawVolume(const model::Volume *volume);
        void _loadDataBuffers(const model::Volume *volume);
        void _loadTextureToShader(const char *uniformName, GLuint textureSlot, const Texture &texture);

      private:
        void _init();
        Framebuffer _output;
        Camera *_camera;
        GLFWwindow *_window;
        InputManager *_inputManager;
        Shader _shader;
        std::map<std::string, BufferCollection> _bufferCollections;
        std::map<std::string, TextureWrapper> _textures;
        std::map<std::string, DirectionLightUniform> _directionLights;
        std::map<std::string, PointLightUniform> _pointLights;

    };

  }  // namespace renderer
}  // namespace leo
