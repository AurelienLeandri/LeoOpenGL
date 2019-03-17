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
    PostProcessNode(OpenGLContext &context, SceneGraph &sceneGraph, Shader &shader, const Camera &camera);
    virtual ~PostProcessNode();

  public:
    virtual void render();
    virtual void notified(Subject *subject, Event event);

  protected:
    virtual void _load();
    virtual void _unload();

  private:
    void _loadPostProcessQuad();

  private:
    const SceneGraph &_sceneGraph;
    const Volume *_postProcessGeometry;
};

} // namespace leo