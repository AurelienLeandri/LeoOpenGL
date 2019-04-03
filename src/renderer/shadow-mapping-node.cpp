#include <renderer/shadow-mapping-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/opengl-context.hpp>
#include <renderer/framebuffer.hpp>

#include <model/scene-graph.hpp>
#include <model/entity.hpp>
#include <model/components/direction-light.hpp>
#include <model/components/volume.hpp>
#include <model/components/transformation.hpp>

namespace leo
{
ShadowMappingNode::ShadowMappingNode(OpenGLContext &context, SceneGraph &sceneGraph, Shader &shader, const DirectionLight &light)
    : RenderNode(context, shader), _light(light), _sceneGraph(sceneGraph)
{
    sceneGraph.watch(this);
}

void ShadowMappingNode::render()
{
    if (!this->_output)
        return;

    glClearColor(0.0, 0.0, 0.0, 1);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // Set to always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_MULTISAMPLE);

    this->_loadShader();

    this->_loadInputFramebuffers();

    this->_loadOutputFramebuffer();

    glEnable(GL_DEPTH_TEST);

    // 1. first render to depth map
    glViewport(0, 0, this->_output->getOptions().width, this->_output->getOptions().height);

    glClear(GL_DEPTH_BUFFER_BIT);

    float near_plane = 1.0f, far_plane = 100.f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::vec3 front = this->_light.direction;
    glm::vec3 pos = -front * ((near_plane + far_plane) * 0.5f);
    glm::vec3 up(0.f, 0.f, 0.f);
    int index = 0;
    for (index; index < 3; index++) {
        if (front[index] != 0.f)
            break;
    }
    up[(index + 1) % 3] = front[index];
    up[index] = -front[(index + 1) % 3];
    up = glm::normalize(up);
    glm::mat4 lightView = glm::lookAt(pos,
                                      pos + front,
                                      up);
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    this->_shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

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
        this->_context.drawVolume(*static_cast<const Volume *>(p_component));
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
                this->_context.loadVAO(*static_cast<Volume *>(c));
                break;
            default:
                break;
            }
            break;
        }
    }
}
} // namespace leo