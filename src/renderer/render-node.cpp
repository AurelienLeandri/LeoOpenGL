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
    for (auto &p : this->_inputs)
    {
        Framebuffer *input = p.second;
        for (const TextureWrapper &tw : input->getColorBuffers())
        {
            glUniform1i(glGetUniformLocation(this->_shader.getProgram(), p.first.c_str()), inputNumber);
            glActiveTexture(GL_TEXTURE0 + inputNumber);
            glBindTexture(GL_TEXTURE_2D, tw.getId());
            inputNumber++;
        }
    }
    this->_materialTextureOffset = inputNumber;
}

void RenderNode::_loadOutputFramebuffer()
{
    this->_context.loadFramebuffer(this->_outputs.size() ? this->_outputs["out"] : nullptr);
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