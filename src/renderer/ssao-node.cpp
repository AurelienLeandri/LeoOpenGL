#include <renderer/ssao-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/camera.hpp>
#include <renderer/opengl-context.hpp>
#include <renderer/scene-context.hpp>
#include <renderer/texture-wrapper.hpp>

#include <model/scene-graph.hpp>

#include <random>

namespace leo
{

SSAONode::SSAONode(OpenGLContext &context, SceneContext &sceneContext, SceneGraph &sceneGraph, Shader &shader, const Camera &camera)
    : RenderNode(context, sceneContext, shader), _sceneGraph(sceneGraph), _camera(camera)
{
    this->_postProcessGeometry = new Volume(Volume::createPostProcessPlane());
    this->_loadPostProcessQuad();

    std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
    std::default_random_engine generator;

    this->_ssaoKernel.push_back(glm::vec3(0, 0, 0));
    for (unsigned int i = 1; i < 64; ++i)
    {
        glm::vec3 sample(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0);
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        //float scale = (float)i / 64.0;
        //scale = 0.1f + (scale * scale) * 0.9f; // larger weight on samples close to the center
        //sample *= scale;
        this->_ssaoKernel.push_back(sample);
    }

    for (unsigned int i = 0; i < 16; i++)
    {
        glm::vec3 noise(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            0.0f);
        this->_ssaoNoise.push_back(noise);
    }

    GLTextureOptions options;
    options.internalFormat = GL_RGB16F;
    options.format = GL_RGB;
    options.type=GL_FLOAT;
    this->_noiseTexture = new TextureWrapper(4, 4, (unsigned char *)(&this->_ssaoNoise[0]), options);
}

SSAONode::~SSAONode()
{
    delete this->_postProcessGeometry;
    delete this->_noiseTexture;
}

void SSAONode::render()
{
    this->_shader.use();
    this->_loadInputFramebuffers();

    this->_shader.setMat4("projection", glm::perspective(this->_camera.getZoom(), (float)1620 / (float)1080, 0.001f, 100.0f));

    glUniform1i(glGetUniformLocation(this->_shader.getProgram(), "noise"), this->_materialTextureOffset);
    glActiveTexture(GL_TEXTURE0 + this->_materialTextureOffset);
    glBindTexture(GL_TEXTURE_2D, this->_noiseTexture->getId());

    glUniform3fv(glGetUniformLocation(this->_shader.getProgram(), "kernel"), 64, &this->_ssaoKernel[0][0]);

    this->_loadOutputFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    this->_context.drawVolume(*this->_postProcessGeometry,
                              this->_sceneContext.bufferCollections.find(this->_postProcessGeometry->getId())->second);
}

void SSAONode::notified(Subject *subject, Event event)
{
}

void SSAONode::_load()
{
}

void SSAONode::_unload()
{
}

void SSAONode::_loadPostProcessQuad()
{
    this->_sceneContext.registerVolume(*this->_postProcessGeometry);
}

} // namespace leo