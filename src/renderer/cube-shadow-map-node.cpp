#include <renderer/cube-shadow-map-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/camera.hpp>
#include <renderer/opengl-context.hpp>
#include <renderer/scene-context.hpp>
#include <renderer/light-wrapper.hpp>

#include <model/scene-graph.hpp>
#include <model/cube-map.hpp>
#include <model/entity.hpp>
#include <model/icomponent.hpp>
#include <model/components/volume.hpp>
#include <model/components/transformation.hpp>
#include <model/components/point-light.hpp>

#include <sstream>

namespace leo
{

CubeShadowMapNode::CubeShadowMapNode(OpenGLContext &context, SceneContext &sceneContext, const SceneGraph &sceneGraph, Shader &shader, const PointLight &light)
    : RenderNode(context, sceneContext, shader), _sceneGraph(sceneGraph), _light(light)
{
    DepthBufferOptions options;
    options.width = 1024;
    options.height = 1024;
    options.type = BufferType::CUBE_MAP;
    this->_output = new Framebuffer();
    this->_output->setDepthBuffer(options);
}

void CubeShadowMapNode::render()
{
    if (!this->_output)
        return;

    glClearColor(1.0, 1.0, 1.0, 1);

    this->_loadShader();

    this->_loadInputFramebuffers();

    this->_loadOutputFramebuffer();

    glEnable(GL_DEPTH_TEST);

    // 1. first render to depth map
    glViewport(0, 0, 1024, 1024);

    glClear(GL_DEPTH_BUFFER_BIT);

    const PointLightWrapper &plw = this->_sceneContext.pLights.find(this->_light.getId())->second;
    const auto &shadowTransforms = plw.shadowTransforms;
    for (int i = 0; i < 6; ++i)
    {
        this->_shader.setMat4(("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);
    }
    this->_shader.setVector3("lightPos", plw.uniform.position);
    this->_shader.setFloat("far_plane", PointLightWrapper::far);

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

void CubeShadowMapNode::_loadShader()
{
    RenderNode::_loadShader();
    this->_shader.use();

    glm::mat4 m;
    this->_shader.setMat4("model", m);
}

void CubeShadowMapNode::notified(Subject *subject, Event event)
{
}

} // namespace leo