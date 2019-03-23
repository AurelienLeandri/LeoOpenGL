#include <renderer/render-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/camera.hpp>
#include <renderer/opengl-context.hpp>

#include <sstream>

namespace leo
{

RenderNode::RenderNode(OpenGLContext &context, Shader &shader, const Camera &camera, RenderNodeOptions options)
    : _context(context), _shader(shader), _camera(camera), _options(options)
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

void RenderNode::_loadShader()
{
}

void RenderNode::_loadTextureToShader(const char *uniformName, GLuint textureSlot, const Texture &texture)
{
    this->_shader.setTexture(uniformName, this->_context.getTextureWrapperId(texture), textureSlot);
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
            std::stringstream number;
            number << inputNumber;
            glUniform1i(glGetUniformLocation(this->_shader.getProgram(), (p.first + number.str()).c_str()), inputNumber);
            glActiveTexture(GL_TEXTURE0 + inputNumber);
            glBindTexture(GL_TEXTURE_2D, cb[i].getId());
            inputNumber++;
        }
    }
    this->_materialTextureOffset = inputNumber;
}

void RenderNode::_loadOutputFramebuffer()
{
    if (this->_output)
    {
        this->_output->loadFrameBuffer();
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
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