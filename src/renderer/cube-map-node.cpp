#include <renderer/cube-map-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/camera.hpp>
#include <renderer/opengl-context.hpp>

#include <model/scene-graph.hpp>
#include <model/cube-map.hpp>

namespace leo
{

BackgroundNode::BackgroundNode(OpenGLContext &context, SceneContext &sceneContext, const SceneGraph &sceneGraph, Shader &shader, const Camera &camera)
    : RenderNode(context, sceneContext, shader), _sceneGraph(sceneGraph), _camera(camera)
{
    this->_cubeMap = sceneGraph.getCubeMap();
}

void BackgroundNode::render()
{
    this->_load();

    this->_shader.use();
    glm::mat4 untranslatedMatrix = glm::mat4(glm::mat3(this->_camera.getViewMatrix()));
    this->_shader.setMat4("view", untranslatedMatrix);
    this->_shader.setMat4("projection", glm::perspective(this->_camera.getZoom(), (float)1620 / (float)1080, 0.1f, 100.0f));
    this->_loadOutputFramebuffer();
    this->_loadInputFramebuffers();

    if (this->_cubeMap)
    {
        glDepthFunc(GL_LEQUAL);

        GLuint VAO = this->_context.loadCubeMap(*this->_cubeMap);
        this->_shader.setTexture(
            "skybox", this->_context.getTextureWrapperId(*this->_cubeMap->getTextures()[0]), this->_materialTextureOffset, 
                this->_cubeMap->getType() == CubeMapType::FACES ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);
    }

    this->_unload();
}

void BackgroundNode::_load()
{
    if (this->_cubeMap)
    {
        this->_loadCubeMap(this->_cubeMap);
    }
}

void BackgroundNode::_unload()
{
    this->_context.loadFramebuffer(nullptr);
}

void BackgroundNode::notified(Subject *subject, Event event)
{
    SceneGraph *c = dynamic_cast<SceneGraph *>(subject);
    if (c && event == Event::CUBE_MAP_UPDATED)
    {
        this->_loadCubeMap(c->getCubeMap());
    }
}

void BackgroundNode::_loadCubeMap(const CubeMap *cubeMap)
{
    this->_cubeMap = cubeMap;
    this->_context.loadCubeMap(*cubeMap);
}

} // namespace leo