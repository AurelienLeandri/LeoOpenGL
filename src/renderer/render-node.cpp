#include <renderer/render-node.hpp>

namespace leo
{

RenderNode::RenderNode(Shader &shader, const Camera &camera) : _shader(shader), _camera(camera)
{
}

std::map<std::string, Framebuffer *> &RenderNode::getInputs()
{
    return this->_inputs;
}

const Framebuffer *RenderNode::getOutput() const
{
    return this->_output;
}

void RenderNode::setOutput(Framebuffer *output)
{
    this->_output = output;
}

} // namespace leo