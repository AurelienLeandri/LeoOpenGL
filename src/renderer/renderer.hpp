#pragma once

#include <renderer/framebuffer.hpp>
#include <renderer/shader.hpp>
#include <renderer/texture-wrapper.hpp>
#include <renderer/light-uniforms.hpp>
#include <renderer/buffer-collection.hpp>
#include <renderer/opengl-context.hpp>

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
class MainNode;
class CubeMapNode;
class PostProcessNode;
class InstancedNode;

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

public:
  void createMainNode(SceneGraph *sceneGraph);
  void createCubeMapNode(SceneGraph *sceneGraph);
  void createPostProcessNode(SceneGraph *sceneGraph);
  void createInstancedNode(SceneGraph *sceneGraph, const std::vector<glm::mat4> &transformations);

private:
  void _loadShader(Shader *shader, std::vector<const Framebuffer *> inputs, Framebuffer *output);
  void _setWindowContext(GLFWwindow *window, InputManager *inputManager);
  void _setCamera(Camera *camera);
  void _initFramebuffers();
  void _loadInstanced(const Instanced *instanced);
  void _getChildrenMeshes(const Entity *root, std::vector<const Volume *> &meshes);

private:
  void _init();

private:
  Framebuffer _main;
  Camera *_camera = nullptr;
  GLFWwindow *_window = nullptr;
  InputManager *_inputManager = nullptr;

  Shader _shader;
  Shader _postProcessShader;
  Shader _cubeMapShader;
  Shader _instancingShader;

  OpenGLContext _context;

  MainNode *_mainNode = nullptr;
  CubeMapNode *_cubeMapNode = nullptr;
  PostProcessNode *_postProcessNode = nullptr;
  InstancedNode *_instancedNode = nullptr;
};

} // namespace leo
