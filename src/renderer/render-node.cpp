#include <renderer/render-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/opengl-context.hpp>
#include <renderer/scene-context.hpp>

#include <utils/texture.hpp>

#include <sstream>

namespace leo
{

RenderNode::RenderNode(OpenGLContext &context, SceneContext &sceneContext, Shader &shader, RenderNodeOptions options)
    : RenderGraphNode(context), _sceneContext(sceneContext), _shader(shader), _options(options)
{
}

void RenderNode::_loadShader()
{
}

void RenderNode::_loadInputFramebuffers()
{
    int inputNumber = 0;
    for (const auto &p : this->_inputs)
    {
        const TextureWrapper &input = p.second;
        glUniform1i(glGetUniformLocation(this->_shader.getProgram(), p.first.c_str()), inputNumber);
        glActiveTexture(GL_TEXTURE0 + inputNumber);
        glBindTexture(GL_TEXTURE_2D, input.getId());
        inputNumber++;
    }
    for (const auto &p : this->_textureUniforms)
    {
        this->_loadTextureToShader(p.first.c_str(), inputNumber++, *p.second);
    }
    this->_materialTextureOffset = inputNumber;
}

void RenderNode::_loadOutputFramebuffer()
{
    this->_context.loadFramebuffer(this->_output);
}

void RenderNode::addTextureUniform(std::string uniformName, const Texture *texture, GLTextureOptions glOptions, TextureOptions textureOptions)
{
    this->_sceneContext.registerTexture(*texture, glOptions, textureOptions);
    this->_textureUniforms.insert(std::pair<std::string, const Texture *>(uniformName, texture));
}

void RenderNode::addTextureUniform(std::string uniformName, const TextureWrapper *textureWrapper)
{
    this->_textureUniforms.insert(std::pair<std::string, const Texture *>(uniformName, textureWrapper->getTexture()));
}

void RenderNode::_loadTextureToShader(const char *uniformName, GLuint textureSlot, const Texture &texture)
{
    this->_shader.setTexture(uniformName, this->_sceneContext.textures.find(texture.getId())->second.getId(), textureSlot);
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