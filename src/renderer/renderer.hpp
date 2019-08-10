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
class BackgroundNode;
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
  Renderer();
  virtual ~Renderer();
  Renderer(const Renderer &other) = delete;

public:
  virtual void notified(Subject *subject, Event event);

public:
  const Renderer &operator=(const Renderer &other) = delete;

public:
  void createSceneContext(OpenGLContext &context); // TODO: Remove, create a separate class to manage observation
  SceneContext &getSceneContext(); // TODO: Remove, create a separate class to manage observation
  void setSceneGraph(SceneGraph &sceneGraph);      // TODO: Remove, create a separate class to manage observation

private:
  void _visitSceneGraph();
  void _visitSceneGraphRec(const Entity &root);
  void _registerComponent(const IComponent &component);
  void _registerDirectionLight(const DirectionLight &dl);

private:
  std::map<t_id, Framebuffer> _directionalShadowMaps;
  std::map<t_id, ShadowMappingNode> _directionalShadowNodes;
  SceneContext *_sceneContext = nullptr;
  SceneGraph *_sceneGraph = nullptr;

public:
  template <typename T, typename... ARGS>
  T *createNode(ARGS &&... args)
  {
    T *node = new T(std::forward<ARGS>(args)...);
    this->_nodes.insert(std::pair<t_id, std::unique_ptr<T>>(node->getId(), node));
    return node;
  }

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

  RenderGraphNode *getNode(t_id id);
  Shader *getShader(t_id id);
  Framebuffer *getFramebuffer(t_id id);
  void execute();

private:
  std::map<t_id, std::unique_ptr<RenderGraphNode>> _nodes;
  std::map<t_id, std::unique_ptr<Framebuffer>> _framebuffers;
  std::map<t_id, std::unique_ptr<Shader>> _shaders;
  Shader _shadowMappingShader;
  Shader _hdrShader;
  Shader _cubeShadowMapShader;
};

} // namespace leo
