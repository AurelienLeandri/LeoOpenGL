#pragma once

#include <map>
#include <vector>
#include <set>

#include <renderer/global.hpp>

#define MAX_NUM_LIGHTS 10

namespace leo
{

using t_RGNodeId = unsigned int;

class Framebuffer;
class OpenGLContext;
class TextureWrapper;
class RenderGraph;

class RenderGraphNode
{
public:
  RenderGraphNode(OpenGLContext &context);

public:
  virtual void render() = 0;

public:
  const std::map<std::string, const TextureWrapper &> &getInputs() const;
  void addInput(RenderGraphNode &in, std::string uniformName, int bufferId);
  Framebuffer *getFramebuffer();
  void setFramebuffer(Framebuffer *fb);
  void addInNode(RenderGraphNode &in);
  void addOutNode(RenderGraphNode &out);
  std::set<t_RGNodeId> &getInNodes();
  std::set<t_RGNodeId> &getOutNodes();
  t_RGNodeId getId() const;

private:
  void _addInNode(RenderGraphNode &in);
  void _addOutNode(RenderGraphNode &out);

protected:
  std::map<std::string, const TextureWrapper &> _inputs;
  std::set<t_RGNodeId> _inNodes;
  std::set<t_RGNodeId> _outNodes;
  Framebuffer *_output = nullptr;
  OpenGLContext &_context;
  t_RGNodeId _id;

private:
  static t_RGNodeId _nbNodes;
};

} // namespace leo