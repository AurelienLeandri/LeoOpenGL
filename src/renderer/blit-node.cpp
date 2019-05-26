#include <renderer/blit-node.hpp>

#include <renderer/opengl-context.hpp>

namespace leo
{

BlitNode::BlitNode(OpenGLContext &context, Framebuffer &input) : RenderGraphNode(context), _input(input)
{
}

void BlitNode::render()
{
    this->_context.loadFramebuffer(&this->_input, GL_READ_FRAMEBUFFER);
    this->_context.loadFramebuffer(this->_output, GL_DRAW_FRAMEBUFFER);
    glBlitFramebuffer(0, 0, 1620, 1080, 0, 0, 1620, 1080,
                      GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
}

} // namespace leo