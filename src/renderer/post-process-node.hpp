#pragma once

#include <renderer/render-node.hpp>

#include <model/components/volume.hpp>

namespace leo
{

class OpenGLContext;
class SceneGraph;
class Shader;
class Camera;
class Volume;

class PostProcessNode : public RenderNode
{
  public:
    PostProcessNode(OpenGLContext &context, SceneContext &sceneContext, SceneGraph &sceneGraph, Shader &shader);
    virtual ~PostProcessNode();

  public:
    virtual void render();
    virtual void notified(Subject *subject, Event event);

  protected:
    void _load();
    void _unload();

  private:
    void _loadPostProcessQuad();

  private:
    const SceneGraph &_sceneGraph;
    const Volume *_postProcessGeometry;
};

} // namespace leo