#pragma once

#include <renderer/framebuffer.hpp>
#include <renderer/shader.hpp>
#include <renderer/texture-wrapper.hpp>
#include <renderer/buffer-collection.hpp>
#include <renderer/opengl-context.hpp>
#include <renderer/blit-node.hpp>
#include <renderer/scene-context.hpp>
#include <renderer/render-graph.hpp>

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
class GaussianBlurNode;
class InstancedNode;
class BlitNode;
class ShadowMappingNode;
class PointLightWrapper;
class DirectionLightWrapper;
class DeferredLightingNode;

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
  void createBlitNode();
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
  //void _executeRenderGraph(std::map<RenderGraphNode *, int> &incompleteInputs, std::map<RenderGraphNode *, bool> &hasRan);

private:
  void _init();

private:
  Framebuffer _main;
  Framebuffer _multisampled;
  Framebuffer _gBuffer;
  Framebuffer _postProcess;
  std::map<t_id, Framebuffer> _directionalShadowMaps;
  std::map<t_id, ShadowMappingNode> _directionalShadowNodes;

  // Framebuffers for bloom effect
  Framebuffer _extractCapedBrightnessFB;
  Framebuffer _hdrCorrectionFB;
  Framebuffer _blurFB;
  Framebuffer _bloomEffectFB;

  Camera *_camera = nullptr;
  GLFWwindow *_window = nullptr;
  InputManager *_inputManager = nullptr;

  Shader _shader;
  Shader _gBufferShader;
  Shader _deferredLightingShader;
  Shader _postProcessShader;
  Shader _cubeMapShader;
  Shader _instancingShader;
  Shader _gammaCorrectionShader;
  Shader _shadowMappingShader;
  Shader _cubeShadowMapShader;

  // Shaders for bloom effect
  Shader _extractCapedBrightnessShader;
  Shader _hdrCorrectionShader;
  Shader _bloomEffectShader;

  OpenGLContext _context;
  SceneContext _sceneContext;

  MainNode *_mainNode = nullptr;
  MainNode *_gBufferNode = nullptr;
  CubeMapNode *_cubeMapNode = nullptr;
  PostProcessNode *_postProcessNode = nullptr;
  PostProcessNode *_gammaCorrectionNode = nullptr;
  DeferredLightingNode *_deferredLightingNode = nullptr;
  InstancedNode *_instancedNode = nullptr;
  BlitNode *_blitNode = nullptr;

  // RenderGraphNodes for bloom effect
  PostProcessNode *_extractCapedBrightnessNode = nullptr;
  GaussianBlurNode *_blurNode = nullptr;
  PostProcessNode *_hdrCorrectionNode = nullptr;
  PostProcessNode *_bloomEffectNode = nullptr;

  SceneGraph &_sceneGraph;

// Render Graph specific stuff
  template <typename... ARGS>
  Framebuffer *createFramebuffer(ARGS &&... args)
  {
    Framebuffer *fb = new Framebuffer(std::forward<ARGS>(args)...);
    this->_framebuffers.insert(std::pair<t_id, std::unique_ptr<Framebuffer>>(fb->getId(), fb));
    return fb;
  }

  template <typename... ARGS>
  Shader *createShader(ARGS &&... args)
  {
    Shader *shader = new Shader(std::forward<ARGS>(args)...);
    this->_shaders.insert(std::pair<t_id, std::unique_ptr<Shader>>(shader->getId(), shader));
    return shader;
  }

  template <typename T, typename... ARGS>
  T *createNode(ARGS &&... args)
  {
    T *node = new T(std::forward<ARGS>(args)...);
    this->_nodes.insert(std::pair<t_id, std::unique_ptr<T>>(node->getId(), node));
    return node;
  }

  RenderGraphNode *getNode(t_id id);
  Shader *getShader(t_id id);
  Framebuffer *getFramebuffer(t_id id);
  OpenGLContext &getOpenGLContext();
  SceneContext &getSceneContext();
  void execute();

private:
  std::map<t_id, std::unique_ptr<RenderGraphNode>> _nodes;
  std::map<t_id, std::unique_ptr<Framebuffer>> _framebuffers;
  std::map<t_id, std::unique_ptr<Shader>> _shaders;
  OpenGLContext _context;
  SceneContext _sceneContext;
};

} // namespace leo
