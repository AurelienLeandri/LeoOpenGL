#pragma once

#include <renderer/render-graph-node.hpp>
#include <controller/observer.hpp>
#include <renderer/render-node-options.hpp>

#include <map>

#include <renderer/global.hpp>

#define MAX_NUM_LIGHTS 10

namespace leo
{

class Framebuffer;
class Camera;
class Shader;
class OpenGLContext;
class Texture;
class SceneContext;

class RenderNode : public Observer, public RenderGraphNode
{
public:
  RenderNode(OpenGLContext &context, SceneContext &sceneContext, Shader &shader, RenderNodeOptions options = {});

public:
  virtual void notified(Subject *subject, Event event) = 0;

protected:
  virtual void _loadShader();
  void _loadTextureToShader(const char *uniformName, GLuint textureSlot, const Texture &texture);
  virtual void _loadOutputFramebuffer();
  virtual void _loadInputFramebuffers();

public:
  void setOptions(RenderNodeOptions options);
  const RenderNodeOptions &getOptions() const;

protected:
  Shader &_shader;
  GLuint _materialTextureOffset = 0;
  RenderNodeOptions _options;
  SceneContext &_sceneContext;

};

} // namespace leo