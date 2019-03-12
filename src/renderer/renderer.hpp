#pragma once

#include <renderer/framebuffer.hpp>
#include <renderer/shader.hpp>
#include <renderer/texture-wrapper.hpp>
#include <renderer/light-uniforms.hpp>
#include <renderer/buffer-collection.hpp>
#include <renderer/opengl-context.hpp>
#include <renderer/main-node.hpp>

#include <model/entity.hpp>

#include <renderer/global.hpp>

#define MAX_NUM_LIGHTS 10

namespace leo
{

class Material;
class CubeMap;
class Transformation;
class DrawableCollection;
class Volume;
class Instanced;

class InputManager;
class Camera;

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
  void render(const SceneGraph *sceneGraph);
  void render(const SceneGraph *sceneGraph,
              std::vector<const Framebuffer *> inputs, Framebuffer *output);

public:
  void createMainNode(SceneGraph *sceneGraph);

private:
  void _renderRec(const Entity *root, Shader *shader, std::vector<const Framebuffer *> inputs, Framebuffer *output, const Instanced *instanced = nullptr);
  void _loadShader(Shader *shader, std::vector<const Framebuffer *> inputs, Framebuffer *output);
  void _drawCubeMap(const CubeMap &cubeMap, Framebuffer *output);
  void _postProcess(Framebuffer *input);
  void _setCurrentMaterial(const Material *material);
  void _setCurrentMaterial(const Material *material, Shader *shader);
  void _setModelMatrix(const Transformation *transformation, Shader *shader);
  void _setModelMatrix(Shader *shader);
  void _setWindowContext(GLFWwindow *window, InputManager *inputManager);
  void _setCamera(Camera *camera);
  void _drawVolume(const Volume *volume, const Instanced *instanced = nullptr);
  void _loadTextureToShader(const char *uniformName, GLuint textureSlot, const Texture &texture);
  void _loadLightsToShader();
  void _loadOutputFramebuffer(Framebuffer *output);
  void _loadInputFramebuffers(std::vector<const Framebuffer *> &inputs, Shader &shader);
  void _initFramebuffers();
  void _loadInstanced(const Instanced *instanced);
  void _getChildrenMeshes(const Entity *root, std::vector<const Volume *> &meshes);
  void _loadLight(const DirectionLight *light);
  void _loadLight(const PointLight *light);

private:
  void _init();
  Framebuffer _main;
  Camera *_camera = nullptr;
  GLFWwindow *_window = nullptr;
  InputManager *_inputManager = nullptr;
  Shader _shader;
  Shader _postProcessShader;
  Shader _cubeMapShader;
  Shader _instancingShader;
  std::map<t_id, DirectionLightUniform> _directionLights;
  std::map<t_id, PointLightUniform> _pointLights;
  GLuint _lightsUBO = 0;
  GLuint _materialTextureOffset = 0;
  Entity _postProcessGeometry;
  OpenGLContext _context;
  MainNode *_mainNode = nullptr;
};

} // namespace leo
