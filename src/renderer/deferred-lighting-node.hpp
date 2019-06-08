#pragma once

#include <renderer/post-process-node.hpp>
#include <renderer/light-uniforms.hpp>
#include <renderer/buffer-collection.hpp>
#include <renderer/texture-wrapper.hpp>
#include <renderer/light-uniforms.hpp>

namespace leo
{

class Transformation;
class SceneGraph;
class Material;
class Entity;
class Volume;
class Texture;
class DirectionLight;
class PointLight;
class SceneContext;

class DeferredLightingNode : public PostProcessNode
{

  using t_id = unsigned int;

public:
  DeferredLightingNode(OpenGLContext &context, SceneContext &sceneContext, SceneGraph &sceneGraph, Shader &shader, const Camera &camera,
           RenderNodeOptions = {GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT});

public:
  virtual void render();
  virtual void _loadInputFramebuffers() override;

protected:
  virtual void _loadShader() override;

protected:
  void _loadLightsToShader();

private:
  GLuint _lightsUBO = 0;
  const SceneGraph &_sceneGraph;
  const Camera &_camera;
  bool _hdr = true;
};

} // namespace leo