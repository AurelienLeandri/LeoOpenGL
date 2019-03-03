#pragma once

#include <renderer/framebuffer.hpp>
#include <renderer/shader.hpp>
#include <renderer/texture-wrapper.hpp>
#include <renderer/light-uniforms.hpp>
#include <model/entity.hpp>

#include <renderer/global.hpp>

#define MAX_NUM_LIGHTS 10

namespace leo
{

namespace model
{
class Material;
class CubeMap;
class Transformation;
class DrawableCollection;
class Volume;
} // namespace model

namespace renderer
{

class InputManager;
class Camera;

typedef struct BufferCollection
{
  GLuint VAO = 0;
  GLuint VBO = 0;
  GLuint EBO = 0;
} BufferCollection;

class Renderer
{
public:
  Renderer(GLFWwindow *window,
           InputManager *inputManager,
           Camera *camera,
           Shader shader);
  virtual ~Renderer();
  Renderer(const Renderer &other) = delete;

public:
  const Renderer &operator=(const Renderer &other) = delete;

public:
  void render(const model::SceneGraph *sceneGraph);
  void render(const model::SceneGraph *sceneGraph,
              std::vector<const Framebuffer *> inputs, Framebuffer *output);

private:
  void _renderRec(const model::Entity *root);
  void _drawCubeMap(const model::CubeMap &cubeMap, Framebuffer *output);
  void _postProcess(Framebuffer *input);
  void _setCurrentMaterial(const model::Material *material);
  void _setModelMatrix(const model::Transformation *transformation);
  void _setModelMatrix();
  void _setWindowContext(GLFWwindow *window, InputManager *inputManager);
  void _setCamera(Camera *camera);
  void _drawVolume(const model::Volume *volume);
  void _loadDataBuffers(const model::Volume *volume);
  void _loadTextureToShader(const char *uniformName, GLuint textureSlot, const Texture &texture);
  void _loadLightsToShader();
  void _registerLightUniforms(const model::Entity *root);
  void _loadCubeMap(const model::CubeMap &cubeMap);
  void _loadOutputFramebuffer(Framebuffer *output);
  void _loadInputFramebuffers(std::vector<const Framebuffer *> &inputs, Shader &shader);
  void _initFramebuffers();

private:
  void _init();
  Framebuffer _main;
  Camera *_camera = nullptr;
  GLFWwindow *_window = nullptr;
  InputManager *_inputManager = nullptr;
  Shader _shader;
  Shader _postProcessShader;
  Shader _cubeMapShader;
  std::map<model::t_id, BufferCollection> _bufferCollections;
  std::map<model::t_id, TextureWrapper> _textures;
  std::map<model::t_id, DirectionLightUniform> _directionLights;
  std::map<model::t_id, PointLightUniform> _pointLights;
  GLuint _lightsUBO = 0;
  GLuint _materialTextureOffset = 0;
  model::Entity _postProcessGeometry;
  BufferCollection _cubeMapBuffer;
};

} // namespace renderer
} // namespace leo
