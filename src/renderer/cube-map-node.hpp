#pragma once

#include <renderer/render-node.hpp>

#include <controller/observer.hpp>

#include <map>

#include <renderer/global.hpp>

#define MAX_NUM_LIGHTS 10

namespace leo
{

class SceneGraph;
class CubeMap;

class CubeMapNode : public RenderNode
{
  public:
    CubeMapNode(OpenGLContext &context, SceneGraph &sceneGraph, Shader &shader, const Camera &camera);

  public:
    virtual void render();
    virtual void notified(Subject *subject, Event event);

  protected:
    void _load();
    void _unload();

  private:
    void _loadCubeMap(const CubeMap *cubeMap);

  private:
    const CubeMap *_cubeMap = nullptr;
    const SceneGraph &_sceneGraph;
};

} // namespace leo