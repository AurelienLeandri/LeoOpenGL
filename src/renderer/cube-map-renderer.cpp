#include <renderer/cube-map-renderer.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/camera.hpp>
#include <renderer/opengl-context.hpp>

#include <model/scene-graph.hpp>
#include <model/cube-map.hpp>

namespace leo
{

CubeMapRenderer::CubeMapRenderer(OpenGLContext &context, SceneGraph &sceneGraph, Shader &shader, const Camera &camera)
    : RenderNode(context, shader, camera), _sceneGraph(sceneGraph)
{
    sceneGraph.watch(this);
}

void CubeMapRenderer::render()
{
    this->_shader.use();
    glm::mat4 untranslatedMatrix = glm::mat4(glm::mat3(this->_camera.getViewMatrix()));
    this->_shader.setMat4("view", untranslatedMatrix);
    this->_shader.setMat4("projection", glm::perspective(this->_camera.getZoom(), (float)1620 / (float)1080, 0.1f, 100.0f));
    this->_loadOutputFramebuffer(output);

    glDepthFunc(GL_LEQUAL);

    GLuint VAO = this->_context.loadCubeMap(this->_cubeMap);
    this->_shader.setTexture(
        "skybox", this->_context.getTextureWrapperId(*this->_cubeMap.getTextures()[0]), 0, GL_TEXTURE_CUBE_MAP);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}

void CubeMapRenderer::_load()
{
    if (this->_cubeMap)
    {
        this->_loadCubeMap(this->_cubeMap);
    }
}

void CubeMapRenderer::_unload()
{
}

void CubeMapRenderer::notified(Subject *subject, Event event)
{
    SceneGraph *c = dynamic_cast<SceneGraph *>(subject);
    if (c && event == Event::CUBE_MAP_UPDATED)
    {
        this->_loadCubeMap(c->getCubeMap());
    }
}

void CubeMapRenderer::_loadCubeMap(const CubeMap *cubeMap)
{
    this->_cubeMap = cubeMap;
    this->_context.loadCubeMap(*cubeMap);
}

} // namespace leo