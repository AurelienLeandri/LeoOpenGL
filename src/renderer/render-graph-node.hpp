#pragma once

#include <map>
#include <string>

#include <renderer/global.hpp>

#define MAX_NUM_LIGHTS 10

namespace leo
{

class Framebuffer;
class OpenGLContext;
class TextureWrapper;

class RenderGraphNode
{
  public:
    RenderGraphNode(OpenGLContext &context);

  public:
    virtual void render() = 0;

  public:
    std::map<std::string, const TextureWrapper &> &getInputs();
    Framebuffer *&getOutput();
    
  protected:
    std::map<std::string, const TextureWrapper &> _inputs;
    Framebuffer * _output = nullptr;
    OpenGLContext &_context;
};

} // namespace leo