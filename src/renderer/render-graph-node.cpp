#include <renderer/render-graph-node.hpp>

#include <renderer/opengl-context.hpp>
#include <renderer/framebuffer.hpp>

namespace leo
{

RenderGraphNode::RenderGraphNode(OpenGLContext &context) : _context(context)
{
}

std::map<std::string, Framebuffer *> &RenderGraphNode::getInputs()
{
    return this->_inputs;
}

std::map<std::string, Framebuffer *> &RenderGraphNode::getOutputs()
{
    return this->_outputs;
}

} // namespace leo