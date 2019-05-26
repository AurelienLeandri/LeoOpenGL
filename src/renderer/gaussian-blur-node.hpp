#pragma once

#include <renderer/post-process-node.hpp>

#include <model/components/volume.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/shader.hpp>


namespace leo
{

class OpenGLContext;
class SceneGraph;
class Camera;
class Volume;

class GaussianBlurNode : public PostProcessNode
{
  public:
    GaussianBlurNode(OpenGLContext &context, SceneContext &sceneContext, SceneGraph &sceneGraph, unsigned int amount = 5);

  public:
    virtual void render() override;

  private:
    Shader _gaussianBlurShader;
    Framebuffer _pingPong;
    unsigned int _amount;
};

} // namespace leo