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

class CubeShadowMapNode : public RenderNode
{
  public:
    CubeShadowMapNode(OpenGLContext &context, SceneContext &sceneContext, const SceneGraph &sceneGraph, Shader &shader, const PointLight &light);

  public:
    virtual void render() override;
    virtual void notified(Subject *subject, Event event);

  private:
    void _renderRec(const Entity *root, const glm::mat4x4 *matrix);
    void _loadShader();

  private:
    const SceneGraph &_sceneGraph;
    const PointLight &_light;
};

} // namespace leo