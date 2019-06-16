#include <renderer/blit-node.hpp>

#include <renderer/opengl-context.hpp>

namespace leo
{

BlitNode::BlitNode(OpenGLContext &context, Framebuffer &input, GLuint blitMask) : RenderGraphNode(context), _input(input),
                                                                                  _blitMask(blitMask)
{
}

void BlitNode::render()
{
    this->_context.loadFramebuffer(&this->_input, GL_READ_FRAMEBUFFER);
    this->_context.loadFramebuffer(this->_output, GL_DRAW_FRAMEBUFFER);
    glBlitFramebuffer(0, 0, 1620, 1080, 0, 0, 1620, 1080,
                      this->_blitMask, GL_NEAREST);
}

} // namespace leo