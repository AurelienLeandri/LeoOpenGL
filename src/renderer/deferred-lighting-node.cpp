#include <renderer/deferred-lighting-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/camera.hpp>
#include <renderer/opengl-context.hpp>
#include <renderer/scene-context.hpp>
#include <renderer/light-wrapper.hpp>

#include <model/components/transformation.hpp>
#include <model/entity.hpp>
#include <model/components/material.hpp>
#include <model/components/volume.hpp>
#include <model/scene-graph.hpp>
#include <model/components/direction-light.hpp>
#include <model/components/point-light.hpp>
#include <model/texture-manager.hpp>

#include <utils/texture.hpp>

#include <sstream>

namespace leo
{

DeferredLightingNode::DeferredLightingNode(OpenGLContext &context, SceneContext &sceneContext, SceneGraph &sceneGraph, Shader &shader, const Camera &camera, RenderNodeOptions options)
    : PostProcessNode(context, sceneContext, sceneGraph, shader), _sceneGraph(sceneGraph), _camera(camera)
{
    { // Lights
        glGenBuffers(1, &this->_lightsUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, this->_lightsUBO);
        auto size = sizeof(PointLightUniform);
        glBufferData(GL_UNIFORM_BUFFER,
                     MAX_NUM_LIGHTS * (sizeof(PointLightUniform) + sizeof(DirectionLightUniform)),
                     NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

void DeferredLightingNode::render()
{
    this->_loadShader();

    this->_loadInputFramebuffers();

    this->_loadOutputFramebuffer();

    this->_shader.setVector3("viewPos", this->_camera.getPosition());
    this->_shader.setVector3("ambientLight", glm::vec3(0.4, 0.4, 0.4));

    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    this->_context.drawVolume(*this->_postProcessGeometry,
                              this->_sceneContext.bufferCollections.find(this->_postProcessGeometry->getId())->second);
}

void DeferredLightingNode::_loadInputFramebuffers()
{
    RenderNode::_loadInputFramebuffers();
    int inputNumber = this->_materialTextureOffset;
    for (auto &p : this->_sceneContext.dLights)
    {
        Framebuffer &input = p.second.map;
        std::stringstream ss;
        ss << inputNumber - this->_materialTextureOffset;
        const TextureWrapper &tw = input.getDepthBuffer();
        glUniform1i(glGetUniformLocation(this->_shader.getProgram(), ("shadowMap" + ss.str()).c_str()), inputNumber);
        glActiveTexture(GL_TEXTURE0 + inputNumber);
        glBindTexture(GL_TEXTURE_2D, tw.getId());
        inputNumber++;
    }
    int cubeMapNb = 0;
    for (auto &p : this->_sceneContext.pLights)
    {
        Framebuffer &input = p.second.map;
        std::stringstream ss;
        ss << cubeMapNb;
        int i = 0;
        const TextureWrapper &tw = input.getDepthBuffer();
        GLuint ul = glGetUniformLocation(this->_shader.getProgram(), ("shadowCubeMap" + ss.str()).c_str());
        glUniform1i(ul, inputNumber);
        glActiveTexture(GL_TEXTURE0 + inputNumber);
        glBindTexture(GL_TEXTURE_CUBE_MAP, tw.getId());
        this->_shader.setVector3(("lightPos" + std::to_string(i)).c_str(), p.second.uniform.position);
        inputNumber++;
        cubeMapNb++;
        i++;
    }
    this->_materialTextureOffset = inputNumber;
}

void DeferredLightingNode::_loadLightsToShader()
{
    unsigned int ubiLights = glGetUniformBlockIndex(this->_shader.getProgram(), "s1");
    if (ubiLights != GL_INVALID_INDEX)
    {
        glUniformBlockBinding(this->_shader.getProgram(), ubiLights, 1);
    }

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->_lightsUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, this->_lightsUBO);
    int i = 0;
    for (auto &p : this->_sceneContext.pLights)
    {
        PointLightUniform &plu = p.second.uniform;
        glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(PointLightUniform), sizeof(PointLightUniform), &plu);
        i++;
    }
    i = 0;
    unsigned int offset = MAX_NUM_LIGHTS * sizeof(PointLightUniform);
    for (auto &p : this->_sceneContext.dLights)
    {
        DirectionLightUniform &dlu = p.second.uniform;
        glBufferSubData(GL_UNIFORM_BUFFER, offset + i * sizeof(DirectionLightUniform), sizeof(DirectionLightUniform), &dlu);
        i++;
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void DeferredLightingNode::_loadShader()
{
    RenderNode::_loadShader();

    this->_shader.use();
    this->_shader.setMat4("view", this->_camera.getViewMatrix());
    this->_shader.setMat4("projection", glm::perspective(this->_camera.getZoom(), (float)1620 / (float)1080, 0.1f, 100.0f));
    this->_shader.setFloat("far_plane", PointLightWrapper::far);

    int matNb = 0;
    for (auto &p : this->_sceneContext.dLights)
    {
        std::stringstream ss;
        ss << matNb;
        this->_shader.setMat4(("lightSpaceMatrix" + ss.str()).c_str(), p.second.projection);
    }

    this->_loadLightsToShader();
}

} // namespace leo