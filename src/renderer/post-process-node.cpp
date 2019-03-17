#include <renderer/post-process-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/camera.hpp>
#include <renderer/opengl-context.hpp>

#include <model/scene-graph.hpp>

namespace leo
{

PostProcessNode::PostProcessNode(OpenGLContext &context, SceneGraph &sceneGraph, Shader &shader, const Camera &camera)
    : RenderNode(context, shader, camera), _sceneGraph(sceneGraph)
{
    this->_postProcessGeometry = new Volume(Volume::createPlane(1.f, 1.f));
    this->_loadPostProcessQuad();
}

PostProcessNode::~PostProcessNode()
{
    delete this->_postProcessGeometry;
}

void PostProcessNode::render()
{
    this->_shader.use();
    this->_loadInputFramebuffers();
    this->_loadOutputFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    this->_context.bindVAO(*this->_postProcessGeometry);
    const std::vector<GLuint> &indices = this->_postProcessGeometry->getIndices();
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(),
                   GL_UNSIGNED_INT, 0);

}

void PostProcessNode::notified(Subject *subject, Event event)
{
}

void PostProcessNode::_load()
{
}

void PostProcessNode::_unload()
{
}

void PostProcessNode::_loadPostProcessQuad()
{
    this->_context.loadVAO(*this->_postProcessGeometry);
}

} // namespace leo