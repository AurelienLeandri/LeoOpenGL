#pragma once

#include <renderer/framebuffer.hpp>
#include <renderer/shader.hpp>
#include <renderer/texture-wrapper.hpp>
#include <renderer/buffer-collection.hpp>
#include <renderer/opengl-context.hpp>
#include <renderer/blit-node.hpp>
#include <renderer/scene-context.hpp>

#include <controller/observer.hpp>

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
class BlitNode;
class ShadowMappingNode;
class PointLightWrapper;
class DirectionLightWrapper;

class Renderer : public Observer
{
public:
  Renderer(GLFWwindow *window,
           InputManager *inputManager,
           Camera *camera,
           Shader shader,
           SceneGraph &sceneGraph);
  virtual ~Renderer();
  Renderer(const Renderer &other) = delete;

public:
  virtual void notified(Subject *subject, Event event);

public:
  const Renderer &operator=(const Renderer &other) = delete;

public:
  void render(const SceneGraph *sceneGraph);

public:
  void createMainNode(SceneGraph *sceneGraph);
  void createCubeMapNode(SceneGraph *sceneGraph);
  void createPostProcessNode(SceneGraph *sceneGraph);
  void createGammaCorrectionNode(SceneGraph *sceneGraph);
  void createInstancedNode(SceneGraph *sceneGraph, const std::vector<glm::mat4> &transformations);

private:
  void _loadShader(Shader *shader, std::vector<const Framebuffer *> inputs, Framebuffer *output);
  void _setWindowContext(GLFWwindow *window, InputManager *inputManager);
  void _setCamera(Camera *camera);
  void _initFramebuffers();
  void _visitSceneGraph();
  void _visitSceneGraphRec(const Entity &root);
  void _registerComponent(const IComponent &component);
  void _registerDirectionLight(const DirectionLight &dl);

private:
  void _init();

private:
  Framebuffer _main;
  Framebuffer _multisampled;
  Framebuffer _postProcess;
  std::map<t_id, Framebuffer> _directionalShadowMaps;
  std::map<t_id, ShadowMappingNode> _directionalShadowNodes;

  Camera *_camera = nullptr;
  GLFWwindow *_window = nullptr;
  InputManager *_inputManager = nullptr;

  Shader _shader;
  Shader _postProcessShader;
  Shader _cubeMapShader;
  Shader _instancingShader;
  Shader _gammaCorrectionShader;
  Shader _shadowMappingShader;
  Shader _cubeShadowMapShader;

  OpenGLContext _context;
  SceneContext _sceneContext;

  MainNode *_mainNode = nullptr;
  CubeMapNode *_cubeMapNode = nullptr;
  PostProcessNode *_postProcessNode = nullptr;
  PostProcessNode *_gammaCorrectionNode = nullptr;
  InstancedNode *_instancedNode = nullptr;
  BlitNode _blitNode;

  SceneGraph &_sceneGraph;
};

} // namespace leo
