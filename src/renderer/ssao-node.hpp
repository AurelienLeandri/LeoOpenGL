#pragma once

#include <renderer/render-node.hpp>

#include <model/components/volume.hpp>

#include <vector>

namespace leo
{

class OpenGLContext;
class SceneGraph;
class Shader;
class Camera;
class Volume;
class TextureWrapper;

class SSAONode : public RenderNode
{
  public:
    SSAONode(OpenGLContext &context, SceneContext &sceneContext, SceneGraph &sceneGraph, Shader &shader, const Camera &camera);
    virtual ~SSAONode();

  public:
    virtual void render();
    virtual void notified(Subject *subject, Event event);

  protected:
    void _load();
    void _unload();

  protected:
    void _loadPostProcessQuad();

  protected:
    const SceneGraph &_sceneGraph;
    const Volume *_postProcessGeometry;
    std::vector<glm::vec3> _ssaoKernel;
    std::vector<glm::vec3> _ssaoNoise;
    TextureWrapper *_noiseTexture;
    const Camera &_camera;
};

} // namespace leo