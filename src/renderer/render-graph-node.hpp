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
    std::map<std::string, Framebuffer *> &getOutputs();
    
  protected:
    std::map<std::string, Framebuffer *> _inputs;
    std::map<std::string, Framebuffer *> _outputs;
    OpenGLContext &_context;
};

} // namespace leo