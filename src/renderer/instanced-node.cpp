#include <renderer/instanced-node.hpp>

#include <renderer/opengl-context.hpp>

#include <model/components/volume.hpp>

namespace leo
{
InstancedNode::InstancedNode(OpenGLContext &context, SceneContext &sceneContext, SceneGraph &sceneGraph, Shader &shader,
                             const Camera &camera, std::vector<glm::mat4> transformations, RenderNodeOptions options)
    : MainNode(context, sceneContext, sceneGraph, shader, camera, options), _transformations(transformations)
{
    glGenBuffers(1, &this->_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
    glBufferData(GL_ARRAY_BUFFER, this->_transformations.size() * sizeof(glm::mat4), &this->_transformations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedNode::_drawVolume(const Volume *volume)
{
    this->_context.bindVAO(*volume);
    const std::vector<GLuint> &indices = volume->getIndices();
    unsigned int amount = this->_transformations.size();
    glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)indices.size(),
                   GL_UNSIGNED_INT, 0, amount);
}

void InstancedNode::_loadVolume(const Volume *volume)
{
    this->_context.loadVAOInstanced(*volume, this->_VBO);
}

} // namespace leo