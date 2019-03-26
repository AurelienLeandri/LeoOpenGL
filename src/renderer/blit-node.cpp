#include <renderer/blit-node.hpp>

#include <renderer/opengl-context.hpp>

namespace leo
{

BlitNode::BlitNode(OpenGLContext &context) : RenderGraphNode(context)
{
}

void BlitNode::render()
{
    if (!this->_inputs.size())
    {
        return; // Nothing to blit
    }
    for (auto &p : this->_inputs)
    {
        auto *input = p.second;
        this->_context.loadFramebuffer(input, GL_READ_FRAMEBUFFER);
        this->_context.loadFramebuffer(this->_output, GL_DRAW_FRAMEBUFFER);
        glBlitFramebuffer(0, 0, 1620, 1080, 0, 0, 1620, 1080,
                          GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
    }
}

} // namespace leo