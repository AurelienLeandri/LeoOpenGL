#include <renderer/main-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/camera.hpp>
#include <renderer/opengl-context.hpp>

#include <model/components/transformation.hpp>
#include <model/entity.hpp>
#include <model/components/material.hpp>
#include <model/components/volume.hpp>
#include <model/scene-graph.hpp>
#include <model/components/direction-light.hpp>
#include <model/components/point-light.hpp>

#include <utils/texture.hpp>

#include <sstream>

namespace leo
{

MainNode::MainNode(OpenGLContext &context, SceneGraph &sceneGraph, Shader &shader, const Camera &camera, RenderNodeOptions options)
    : RenderNode(context, shader, camera, options), _sceneGraph(sceneGraph)
{
    sceneGraph.watch(this);
    this->_loadAllLightsFromSceneGraph();
    { // Lights
        glGenBuffers(1, &this->_lightsUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, this->_lightsUBO);
        auto size = sizeof(PointLightUniform);
        glBufferData(GL_UNIFORM_BUFFER,
                     MAX_NUM_LIGHTS * (sizeof(PointLightUniform) + sizeof(DirectionLightUniform)),
                     NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

MainNode::~MainNode()
{
    if (this->_multiSampledFramebuffer)
    {
        delete this->_multiSampledFramebuffer;
    }
}

void MainNode::render()
{
    this->_load();
    this->_renderRec(this->_sceneGraph.getRoot());
    this->_unload();
}

void MainNode::enableMultiSampling(unsigned int nbSamples)
{
    this->disableMultiSampling();
    FramebufferOptions options;
    options.multiSampled = true;
    options.nbSamples = nbSamples;
    this->_multiSampledFramebuffer = new Framebuffer(options);
}

void MainNode::disableMultiSampling()
{
    if (this->_multiSampledFramebuffer)
    {
        delete this->_multiSampledFramebuffer;
        this->_multiSampledFramebuffer = nullptr;
    }
}

void MainNode::_renderRec(const Entity *root)
{
    const IComponent *p_component;
    p_component = root->getComponent(ComponentType::MATERIAL);
    if (p_component)
    {
        this->_setCurrentMaterial(static_cast<const Material *>(p_component));
    }
    p_component = root->getComponent(ComponentType::TRANSFORMATION);
    if (p_component)
    {
        this->_setModelMatrix(static_cast<const Transformation *>(p_component));
    }
    // NOTE: Keep volume at the end as it is affected by the transform and material
    p_component = root->getComponent(ComponentType::VOLUME);
    if (p_component)
    {
        this->_drawVolume(static_cast<const Volume *>(p_component));
    }

    for (auto &child : root->getChildren())
        this->_renderRec(child.second);
}

void MainNode::_drawVolume(const Volume *volume)
{
    this->_context.bindVAO(*volume);
    const std::vector<GLuint> &indices = volume->getIndices();
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(),
                   GL_UNSIGNED_INT, 0);
}

void MainNode::_load()
{
    glClearColor(0.07, 0.07, 0.07, 1);

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

    glClear(this->_options.clearBufferFlags);
    glEnable(GL_DEPTH_TEST);
}

void MainNode::_loadLightsToShader()
{
    unsigned int ubiLights = glGetUniformBlockIndex(this->_shader.getProgram(), "s1");
    if (ubiLights != GL_INVALID_INDEX)
    {
        glUniformBlockBinding(this->_shader.getProgram(), ubiLights, 1);
    }

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->_lightsUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, this->_lightsUBO);
    int i = 0;
    for (auto &p : this->_pointLights)
    {
        PointLightUniform &plu = p.second;
        glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(PointLightUniform), sizeof(PointLightUniform), &plu);
        i++;
    }
    i = 0;
    unsigned int offset = MAX_NUM_LIGHTS * sizeof(PointLightUniform);
    for (auto &p : this->_directionLights)
    {
        DirectionLightUniform &dlu = p.second;
        glBufferSubData(GL_UNIFORM_BUFFER, offset + i * sizeof(DirectionLightUniform), sizeof(DirectionLightUniform), &dlu);
        i++;
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void MainNode::_setModelMatrix(const Transformation *transformation)
{
    this->_shader.setMat4("model", transformation->getTransformationMatrix());
}

void MainNode::_setModelMatrix()
{
    glm::mat4 m;
    this->_shader.setMat4("model", m);
}

void MainNode::_setCurrentMaterial(const Material *material)
{
    this->_shader.setVector3("material.diffuse_value", material->diffuse_value);
    if (material->diffuse_texture)
    {
        this->_loadTextureToShader("material.diffuse_texture", this->_materialTextureOffset + 0, *material->diffuse_texture);
    }

    this->_shader.setVector3("material.specular_value", material->specular_value);
    this->_shader.setInt("material.shininess", material->shininess);
    if (material->specular_texture)
    {
        this->_loadTextureToShader("material.specular_texture", this->_materialTextureOffset + 1, *material->specular_texture);
    }

    if (material->reflection_map)
    {
        this->_loadTextureToShader("material.reflection_map", this->_materialTextureOffset + 2, *material->reflection_map);
    }
}

void MainNode::_loadAllLightsFromSceneGraph()
{
    for (auto &pair : this->_sceneGraph.getDirectionLights())
    {
        this->_loadLight(pair.second);
    }
    for (auto &pair : this->_sceneGraph.getPointLights())
    {
        this->_loadLight(pair.second);
    }
}

void MainNode::_loadLight(const DirectionLight *light)
{
    this->_directionLights.insert(std::pair<t_id, DirectionLightUniform>(light->getId(), DirectionLightUniform(*light)));
    DirectionLightUniform &dlu = this->_directionLights[light->getId()];
    const Transformation *transform = static_cast<const Transformation *>(light->getEntity()->getComponent(ComponentType::TRANSFORMATION));
    if (transform)
    {
        const glm::mat4x4 &transformation = transform->getTransformationMatrix();
        dlu.direction = transformation * light->direction;
    }
}

void MainNode::_loadLight(const PointLight *light)
{
    this->_pointLights.insert(std::pair<t_id, PointLightUniform>(light->getId(), PointLightUniform(*light)));
    PointLightUniform &plu = this->_pointLights[light->getId()];
    const Transformation *transform = static_cast<const Transformation *>(light->getEntity()->getComponent(ComponentType::TRANSFORMATION));
    if (transform)
    {
        const glm::mat4x4 &transformation = transform->getTransformationMatrix();
        plu.position = transformation * light->position;
    }
}

void MainNode::_loadVolume(const Volume *volume)
{
    this->_context.loadVAO(*volume);
}

void MainNode::_loadShader()
{
    RenderNode::_loadShader();

    this->_shader.use();
    this->_shader.setMat4("view", this->_camera.getViewMatrix());
    this->_shader.setMat4("projection", glm::perspective(this->_camera.getZoom(), (float)1620 / (float)1080, 0.1f, 100.0f));

    this->_loadLightsToShader();
    this->_setModelMatrix();
}

void MainNode::_unload()
{
}

void MainNode::notified(Subject *subject, Event event)
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
                this->_loadVolume(static_cast<Volume *>(c));
                break;
            default:
                break;
            }
            break;
        case ComponentType::POINT_LIGHT:
            switch (event)
            {
            case Event::COMPONENT_ADDED:
                this->_loadLight(static_cast<PointLight *>(c));
                break;
            default:
                break;
            }
            break;
        case ComponentType::DIRECTION_LIGHT:
            switch (event)
            {
            case Event::COMPONENT_ADDED:
                this->_loadLight(static_cast<DirectionLight *>(c));
                break;
            default:
                break;
            }
            break;
        }
    }
}

} // namespace leo