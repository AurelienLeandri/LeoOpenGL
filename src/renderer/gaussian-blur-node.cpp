#include <renderer/gaussian-blur-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/camera.hpp>
#include <renderer/opengl-context.hpp>
#include <renderer/scene-context.hpp>

#include <model/scene-graph.hpp>

#include <iostream>

namespace leo
{

GaussianBlurNode::GaussianBlurNode(OpenGLContext &context, SceneContext &sceneContext, SceneGraph &sceneGraph, unsigned int amount)
    : _gaussianBlurShader("resources/shaders/post-process.vs.glsl", "resources/shaders/blur.frag.glsl"),
      PostProcessNode(context, sceneContext, sceneGraph, _gaussianBlurShader), _amount(amount)
{
    this->_pingPong.addColorBuffer();
    this->_pingPong.useRenderBuffer();
}

void GaussianBlurNode::render()
{
    if (this->_inputs.size() != 1)
    {
        std::cerr << "GaussianBlurNode: Takes exactly one input" << std::endl;
        return;
    }

    this->_shader.use();

    bool horizontal = true;

    const TextureWrapper *inputs[2] = {&this->_output->getColorBuffers()[0], &this->_pingPong.getColorBuffers()[0]};
    const Framebuffer *outputs[2] = {&this->_pingPong, this->_output};

    for (unsigned int i = 0; i < this->_amount * 2; ++i)
    {
        this->_shader.setInt("horizontal", horizontal);

        glUniform1i(glGetUniformLocation(this->_shader.getProgram(), "fb"), 0);
        glActiveTexture(GL_TEXTURE0);
        if (i == 0)
            glBindTexture(GL_TEXTURE_2D, this->_inputs.begin()->second.getId());  // First time we use the input buffer
        else
            glBindTexture(GL_TEXTURE_2D, inputs[(i) % 2]->getId());

        this->_context.loadFramebuffer(outputs[(i + 1) % 2]);

        this->_context.drawVolume(*this->_postProcessGeometry,
                                  this->_sceneContext.bufferCollections.find(this->_postProcessGeometry->getId())->second);

        horizontal != horizontal;
    }

    this->_context.loadFramebuffer(nullptr);
}

} // namespace leo