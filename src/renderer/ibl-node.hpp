#pragma once

#include <renderer/render-node.hpp>

#include <controller/observer.hpp>

#include <map>

#include <renderer/global.hpp>

#define MAX_NUM_LIGHTS 10

namespace leo
{

class PointLight;
class Entity;
class SceneGraph;
class CubeMap;
class Transformation;
class IBL;

class IBLNode : public RenderNode
{
public:
  IBLNode(OpenGLContext &context, SceneContext &sceneContext, const SceneGraph &sceneGraph, Shader &shader, const IBL &ibl);

public:
  virtual void render() override;
  virtual void notified(Subject *subject, Event event);

public:
  GLuint getTextureId() const;

private:
  const SceneGraph &_sceneGraph;
  const IBL &_ibl;
  GLuint _textureId;
};

} // namespace leo