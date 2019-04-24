#pragma once

#include <renderer/render-node.hpp>
#include <controller/observer.hpp>

namespace leo
{

class DirectionLight;
class Entity;
class SceneGraph;

class ShadowMappingNode : public RenderNode
{
  public:
    ShadowMappingNode(OpenGLContext &context, SceneContext &sceneContext, const SceneGraph &sceneGraph, Shader &shader, const DirectionLight &light);

  public:
    virtual void render() override;
    virtual void notified(Subject *subject, Event event) override;
    void setLightSpaceMatrix(glm::mat4x4 lightSpaceMatrix);

  private:
    void _renderRec(const Entity *root, const glm::mat4x4 *matrix);
    virtual void _loadShader() override;

  private:
    const DirectionLight &_light;
    const SceneGraph &_sceneGraph;
    glm::mat4x4 _lightSpaceMatrix;
};
} // namespace leo