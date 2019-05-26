#include <renderer/render-graph-node.hpp>

#include <renderer/opengl-context.hpp>
#include <renderer/framebuffer.hpp>

namespace leo
{

RenderGraphNode::RenderGraphNode(OpenGLContext &context) : _context(context)
{
}

std::map<std::string, const TextureWrapper &> &RenderGraphNode::getInputs()
{
    return this->_inputs;
}

Framebuffer *&RenderGraphNode::getOutput()
{
    return this->_output;
}

} // namespace leo