#include <renderer/instanced-node.hpp>

#include <renderer/opengl-context.hpp>
#include <renderer/scene-context.hpp>

#include <model/components/volume.hpp>

namespace leo
{
InstancedNode::InstancedNode(OpenGLContext &context, SceneContext &sceneContext, SceneGraph &sceneGraph, Shader &shader,
                             const Camera &camera, std::vector<glm::mat4> transformations, RenderNodeOptions options)
    : MainNode(context, sceneContext, sceneGraph, shader, camera, options), _transformations(transformations)
{
    this->_VBO = this->_sceneContext.instancingVBO;
}

void InstancedNode::_drawVolume(const Volume *volume)
{
    this->_context.drawVolumeInstanced(*volume,
                                       this->_sceneContext.bufferCollectionsInstanced.find(volume->getId())->second,
                                       this->_transformations.size());
}

} // namespace leo