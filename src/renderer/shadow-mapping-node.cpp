#include <renderer/shadow-mapping-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/opengl-context.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/scene-context.hpp>

#include <model/scene-graph.hpp>
#include <model/entity.hpp>
#include <model/components/direction-light.hpp>
#include <model/components/volume.hpp>
#include <model/components/transformation.hpp>

namespace leo
{
ShadowMappingNode::ShadowMappingNode(OpenGLContext &context, SceneContext &sceneContext, const SceneGraph &sceneGraph, Shader &shader, const DirectionLight &light)
    : RenderNode(context, sceneContext, shader), _light(light), _sceneGraph(sceneGraph)
{
}

void ShadowMappingNode::render()
{
    if (!this->_outputs.size())
        return;

    glClearColor(1.0, 1.0, 1.0, 1);

    this->_loadShader();

    this->_loadInputFramebuffers();

    this->_loadOutputFramebuffer();

    glEnable(GL_DEPTH_TEST);

    // 1. first render to depth map
    glViewport(0, 0, 1620 * 2, 1080 * 2);

    glClear(GL_DEPTH_BUFFER_BIT);

    this->_shader.setMat4("lightSpaceMatrix", this->_lightSpaceMatrix);

    glm::mat4x4 m;
    this->_renderRec(this->_sceneGraph.getRoot(), &m);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 2. then render scene as normal with shadow mapping (using depth map)
    glViewport(0, 0, 1620, 1080);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ShadowMappingNode::_renderRec(const Entity *root, const glm::mat4x4 *matrix)
{
    const glm::mat4x4 *newMatrix = matrix;
    const IComponent *p_component;
    p_component = root->getComponent(ComponentType::TRANSFORMATION);
    if (p_component)
    {
        newMatrix = &(static_cast<const Transformation *>(p_component))->getTransformationMatrix();
        this->_shader.setMat4("model", *newMatrix);
    }

    p_component = root->getComponent(ComponentType::VOLUME);
    if (p_component)
    {
        this->_context.drawVolume(*static_cast<const Volume *>(p_component),
                                  this->_sceneContext.bufferCollections.find(p_component->getId())->second);
    }

    for (auto &child : root->getChildren())
        this->_renderRec(child.second, newMatrix);
    if (newMatrix != matrix)
        this->_shader.setMat4("model", *matrix);
}

void ShadowMappingNode::_loadShader()
{
    RenderNode::_loadShader();
    this->_shader.use();

    glm::mat4 m;
    this->_shader.setMat4("model", m);
}

void ShadowMappingNode::setLightSpaceMatrix(glm::mat4x4 lightSpaceMatrix)
{
    this->_lightSpaceMatrix = lightSpaceMatrix;
}

void ShadowMappingNode::notified(Subject *subject, Event event)
{
    IComponent *c = dynamic_cast<IComponent *>(subject);
    if (c)
    {
        switch (c->getTypeId())
        {
        case ComponentType::VOLUME:
            switch (event)
            {
            case Event::COMPONENT_ADDED:
                //this->_context.loadVAO(*static_cast<Volume *>(c));
                break;
            default:
                break;
            }
            break;
        }
    }
}
} // namespace leo