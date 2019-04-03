#include <renderer/render-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/opengl-context.hpp>

#include <sstream>

namespace leo
{

RenderNode::RenderNode(OpenGLContext &context, Shader &shader, RenderNodeOptions options)
    : RenderGraphNode(context), _shader(shader), _options(options)
{
}

void RenderNode::_loadShader()
{
}

void RenderNode::_loadInputFramebuffers()
{
    int inputNumber = 0;
    for (auto &p : this->_inputs)
    {
        Framebuffer *input = p.second;
        const std::vector<TextureWrapper> &cb = input->getColorBuffers();
        for (GLuint i = 0; i < cb.size(); i++)
        {
            glUniform1i(glGetUniformLocation(this->_shader.getProgram(), p.first.c_str()), inputNumber);
            glActiveTexture(GL_TEXTURE0 + inputNumber);
            glBindTexture(GL_TEXTURE_2D, cb[i].getId());
            inputNumber++;
        }
    }
    this->_materialTextureOffset = inputNumber;
}

void RenderNode::_loadOutputFramebuffer()
{
    this->_context.loadFramebuffer(this->_output);
}

void RenderNode::_loadTextureToShader(const char *uniformName, GLuint textureSlot, const Texture &texture)
{
    this->_shader.setTexture(uniformName, this->_context.getTextureWrapperId(texture), textureSlot);
}

void RenderNode::setOptions(RenderNodeOptions options)
{
    this->_options = options;
}

const RenderNodeOptions &RenderNode::getOptions() const
{
    return this->_options;
}

} // namespace leo