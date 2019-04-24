#include <renderer/post-process-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/camera.hpp>
#include <renderer/opengl-context.hpp>
#include <renderer/scene-context.hpp>

#include <model/scene-graph.hpp>

namespace leo
{

PostProcessNode::PostProcessNode(OpenGLContext &context, SceneContext &sceneContext, SceneGraph &sceneGraph, Shader &shader)
    : RenderNode(context, sceneContext, shader), _sceneGraph(sceneGraph)
{
    this->_postProcessGeometry = new Volume(Volume::createPostProcessPlane());
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

    this->_context.drawVolume(*this->_postProcessGeometry,
                              this->_sceneContext.bufferCollections.find(this->_postProcessGeometry->getId())->second);
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
    this->_sceneContext.registerVolume(*this->_postProcessGeometry);
}

} // namespace leo