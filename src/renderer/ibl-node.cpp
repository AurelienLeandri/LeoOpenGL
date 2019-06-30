#include <renderer/ibl-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/camera.hpp>
#include <renderer/opengl-context.hpp>
#include <renderer/scene-context.hpp>
#include <renderer/light-wrapper.hpp>
#include <renderer/ibl-wrapper.hpp>

#include <model/scene-graph.hpp>
#include <model/cube-map.hpp>
#include <model/entity.hpp>
#include <model/icomponent.hpp>
#include <model/components/volume.hpp>
#include <model/components/transformation.hpp>
#include <model/components/point-light.hpp>

#include <sstream>

namespace leo
{

IBLNode::IBLNode(OpenGLContext &context, SceneContext &sceneContext, const SceneGraph &sceneGraph, Shader &shader, const IBL &ibl)
    : RenderNode(context, sceneContext, shader), _sceneGraph(sceneGraph), _ibl(ibl)
{
}

void IBLNode::render()
{
    Volume cube = Volume::createCube();
    this->_sceneContext.registerVolume(cube);

    glGenTextures(1, &this->_textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->_textureId);
    for (unsigned int i = 0; i < 6; ++i)
    {
        // note that we store each face with 16 bit floating point values
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                     1024, 1024, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // convert HDR equirectangular environment map to cubemap equivalent
    this->_shader.use();

    const IBLWrapper &wrapper = this->_sceneContext.ibls.find(this->_ibl.getId())->second;

    this->_shader.setInt("hdr", 0);
    this->_shader.setMat4("projection", wrapper.projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wrapper.tw.getId());

    glViewport(0, 0, 1024, 1024); // don't forget to configure the viewport to the capture dimensions.

    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 1024);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        this->_shader.setMat4("view", wrapper.views[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->_textureId, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->_context.drawVolume(cube,
                                  this->_sceneContext.bufferCollections.find(cube.getId())->second);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. then render scene as normal with shadow mapping (using depth map)
    glViewport(0, 0, 1620, 1080);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void IBLNode::notified(Subject *subject, Event event)
{
}

GLuint IBLNode::getTextureId() const
{
    return this->_textureId;
}

} // namespace leo