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

const Framebuffer *RenderGraphNode::getOutput() const
{
    return this->_output;
}

void RenderGraphNode::setOutput(Framebuffer *output)
{
    this->_output = output;
}

} // namespace leo