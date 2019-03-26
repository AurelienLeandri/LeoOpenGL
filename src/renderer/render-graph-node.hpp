#pragma once

#include <map>

#include <renderer/global.hpp>

#define MAX_NUM_LIGHTS 10

namespace leo
{

class Framebuffer;
class OpenGLContext;

class RenderGraphNode
{
  public:
    RenderGraphNode(OpenGLContext &context);

  public:
    virtual void render() = 0;

  public:
    std::map<std::string, Framebuffer *> &getInputs();
    const Framebuffer *getOutput() const;
    void setOutput(Framebuffer *output);

  protected:
    std::map<std::string, Framebuffer *> _inputs;
    Framebuffer *_output = nullptr;
    OpenGLContext &_context;
};

} // namespace leo