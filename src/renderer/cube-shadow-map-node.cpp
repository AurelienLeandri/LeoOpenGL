#include <renderer/cube-shadow-map-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/camera.hpp>
#include <renderer/opengl-context.hpp>
#include <renderer/scene-context.hpp>

#include <model/scene-graph.hpp>
#include <model/cube-map.hpp>
#include <model/entity.hpp>
#include <model/icomponent.hpp>
#include <model/components/volume.hpp>
#include <model/components/transformation.hpp>

#include <sstream>

namespace leo
{

CubeShadowMapNode::CubeShadowMapNode(OpenGLContext &context, SceneContext &sceneContext, const SceneGraph &sceneGraph, Shader &shader, const PointLight &light)
    : RenderNode(context, sceneContext, shader), _sceneGraph(sceneGraph), _light(light)
{
    FramebufferOptions options;
    options.width = 1620 * 2;
    options.height = 1080 * 2;
    options.type = FrameBufferType::DEPTH_MAP;

    for (int i = 0; i < 6; i++)
    {
        std::stringstream ss;
        ss << "out";
        if (i > 0)
            ss << i;
        FramebufferOptions options;
        options.width = 1620 * 2;
        options.height = 1080 * 2;
        options.type = FrameBufferType::DEPTH_MAP;
        this->_outputs[ss.str()] = new Framebuffer(options);
        this->_outputs[ss.str()]->generate();
    }
}

void CubeShadowMapNode::render()
{
    if (!this->_outputs.size())
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
    glViewport(0, 0, this->_outputs["out0"]->getOptions().width, this->_outputs["out0"]->getOptions().height);

    glClear(GL_DEPTH_BUFFER_BIT);

    //this->_shader.setMat4("lightSpaceMatrix", this->_lightSpaceMatrix);

    glm::mat4x4 m;
    this->_renderRec(this->_sceneGraph.getRoot(), &m);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 2. then render scene as normal with shadow mapping (using depth map)
    glViewport(0, 0, 1620, 1080);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CubeShadowMapNode::_renderRec(const Entity *root, const glm::mat4x4 *matrix)
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

void CubeShadowMapNode::notified(Subject *subject, Event event)
{
}

} // namespace leo