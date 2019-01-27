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

#define MAX_NUM_LIGHTS 10

namespace leo {

  namespace model {
    class Material;
    class CubeMap;
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

      public:
        void render(const model::SceneGraph *sceneGraph);
        void render(const model::SceneGraph *sceneGraph,
            std::vector<const Framebuffer *> inputs, Framebuffer *output);

      private:
        void _renderRec(const model::Base *root,
            std::vector<const Framebuffer *> inputs, Framebuffer *output);
        void _setCurrentMaterial(model::Material *material);
        void _setModelMatrix(model::Transformation *transformation);
        void _setModelMatrix();
        void _setWindowContext(GLFWwindow *window, InputManager *inputManager);
        void _setCamera(Camera *camera);
        void _drawCollection(model::DrawableCollection *collection);
        void _drawVolume(const model::Volume *volume);
        void _loadDataBuffers(const model::Volume *volume);
        void _loadTextureToShader(const char *uniformName, GLuint textureSlot, const Texture &texture);
        void _loadLightsToShader();
        void _registerLightUniforms(const model::Base *root);
        void _loadCubeMap(const model::CubeMap *cubeMap);
        void _loadOutputFramebuffer(Framebuffer *output);
        void _loadInputFramebuffers(std::vector<const Framebuffer *> &inputs);
        void _initFramebuffers();

      private:
        void _init();
        Framebuffer _main;
        Camera *_camera;
        GLFWwindow *_window;
        InputManager *_inputManager;
        Shader _shader;
        std::map<std::string, BufferCollection> _bufferCollections;
        std::map<std::string, TextureWrapper> _textures;
        std::map<std::string, DirectionLightUniform> _directionLights;
        std::map<std::string, PointLightUniform> _pointLights;
        GLuint _lightsUBO;
        GLuint _materialTextureOffset = 0;

    };

  }  // namespace renderer
}  // namespace leo
