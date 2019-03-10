#pragma once

#include <map>

#include <renderer/global.hpp>

#define MAX_NUM_LIGHTS 10

namespace leo
{

class Framebuffer;
class Camera;
class Shader;

class RenderNode
{
  public:
    RenderNode(Shader &shader, const Camera &camera);

  public:
    virtual void render() = 0;

  protected:
    virtual void _load() = 0;
    virtual void _unload() = 0;
    void _initFramebuffers();

  public:
    std::map<std::string, Framebuffer *> &getInputs();
    const Framebuffer *getOutput() const;
    void setOutput(Framebuffer *output);

  protected:
    Shader &_shader;
    const Camera &_camera;
    std::map<std::string, Framebuffer *> _inputs;
    Framebuffer *_output = nullptr;
};

} // namespace leo