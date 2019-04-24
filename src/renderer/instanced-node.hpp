#pragma once

#include <renderer/main-node.hpp>

#include <vector>

namespace leo
{

class InstancedNode : public MainNode
{
  public:
    InstancedNode(OpenGLContext &context, SceneContext &sceneContext, SceneGraph &sceneGraph, Shader &shader,
    const Camera &camera, std::vector<glm::mat4> transformations,
    RenderNodeOptions options = {});

  private:
    virtual void _drawVolume(const Volume *volume) override;

  private:
    std::vector<glm::mat4> _transformations;
    GLuint _VBO = 0;
};

} // namespace leo