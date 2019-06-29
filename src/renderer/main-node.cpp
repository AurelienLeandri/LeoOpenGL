#include <renderer/main-node.hpp>

#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/camera.hpp>
#include <renderer/opengl-context.hpp>
#include <renderer/scene-context.hpp>
#include <renderer/light-wrapper.hpp>

#include <model/components/transformation.hpp>
#include <model/entity.hpp>
#include <model/components/material.hpp>
#include <model/components/pbr-material.hpp>
#include <model/components/volume.hpp>
#include <model/scene-graph.hpp>
#include <model/components/direction-light.hpp>
#include <model/components/point-light.hpp>
#include <model/texture-manager.hpp>

#include <utils/texture.hpp>

#include <sstream>

namespace leo
{

MainNode::MainNode(OpenGLContext &context, SceneContext &sceneContext, SceneGraph &sceneGraph, Shader &shader, const Camera &camera, RenderNodeOptions options)
    : RenderNode(context, sceneContext, shader, options), _sceneGraph(sceneGraph), _camera(camera)
{
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

void MainNode::render()
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

    glm::mat4x4 m;
    Material defaultMat;
    PBRMaterial defaultPBRMat;
    this->_shader.setVector3("viewPos", this->_camera.getPosition());
    this->_shader.setVector3("ambientLight", glm::vec3(0.4, 0.4, 0.4));
    this->_renderRec(this->_sceneGraph.getRoot(), &defaultMat, &defaultPBRMat, &m);
}

void MainNode::_loadInputFramebuffers()
{
    RenderNode::_loadInputFramebuffers();
    int inputNumber = this->_materialTextureOffset;
    for (auto &p : this->_sceneContext.dLights)
    {
        Framebuffer &input = p.second.map;
        std::stringstream ss;
        ss << inputNumber - this->_materialTextureOffset;
        const TextureWrapper &tw = input.getDepthBuffer();
        glUniform1i(glGetUniformLocation(this->_shader.getProgram(), ("shadowMap" + ss.str()).c_str()), inputNumber);
        glActiveTexture(GL_TEXTURE0 + inputNumber);
        glBindTexture(GL_TEXTURE_2D, tw.getId());
        inputNumber++;
    }
    int cubeMapNb = 0;
    for (auto &p : this->_sceneContext.pLights)
    {
        Framebuffer &input = p.second.map;
        std::stringstream ss;
        ss << cubeMapNb;
        int i = 0;
        const TextureWrapper &tw = input.getDepthBuffer();
        GLuint ul = glGetUniformLocation(this->_shader.getProgram(), ("shadowCubeMap" + ss.str()).c_str());
        glUniform1i(ul, inputNumber);
        glActiveTexture(GL_TEXTURE0 + inputNumber);
        glBindTexture(GL_TEXTURE_CUBE_MAP, tw.getId());
        this->_shader.setVector3(("lightPos" + std::to_string(i)).c_str(), p.second.uniform.position);
        inputNumber++;
        cubeMapNb++;
        i++;
    }
    this->_materialTextureOffset = inputNumber;
}

// TODO: make both materials inherit from a base class
void MainNode::_renderRec(const Entity *root, const Material *material, const PBRMaterial *PBRmaterial, const glm::mat4x4 *matrix)
{
    const Material *newMaterial = material;
    const PBRMaterial *newPBRMaterial = PBRmaterial;
    const glm::mat4x4 *newMatrix = matrix;
    const IComponent *p_component;
    p_component = root->getComponent(ComponentType::MATERIAL);
    if (p_component)
    {
        newMaterial = static_cast<const Material *>(p_component);
        this->_setCurrentMaterial(newMaterial);
    }
    p_component = root->getComponent(ComponentType::PBR_MATERIAL);
    if (p_component)
    {
        newPBRMaterial = static_cast<const PBRMaterial *>(p_component);
        this->_setCurrentMaterial(newPBRMaterial);
    }
    p_component = root->getComponent(ComponentType::TRANSFORMATION);
    if (p_component)
    {
        newMatrix = &(static_cast<const Transformation *>(p_component))->getTransformationMatrix();
        this->_setModelMatrix(newMatrix);
    }
    // NOTE: Keep volume at the end as it is affected by the transform and material
    p_component = root->getComponent(ComponentType::VOLUME);
    if (p_component)
    {
        this->_drawVolume(static_cast<const Volume *>(p_component));
    }

    for (auto &child : root->getChildren())
    {
        this->_renderRec(child.second, newMaterial, newPBRMaterial, newMatrix);
    }
    if (newMatrix != matrix)
    {
        this->_setModelMatrix(matrix);
    }
    if (newMaterial != material)
    {
        this->_setCurrentMaterial(material);
    }
    if (newPBRMaterial != PBRmaterial)
    {
        this->_setCurrentMaterial(PBRmaterial);
    }
}

void MainNode::_drawVolume(const Volume *volume)
{
    if (this->_label == volume->getLabel())
        this->_context.drawVolume(*volume, this->_sceneContext.bufferCollections.find(volume->getId())->second);
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
    for (auto &p : this->_sceneContext.pLights)
    {
        PointLightUniform &plu = p.second.uniform;
        glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(PointLightUniform), sizeof(PointLightUniform), &plu);
        i++;
    }
    i = 0;
    unsigned int offset = MAX_NUM_LIGHTS * sizeof(PointLightUniform);
    for (auto &p : this->_sceneContext.dLights)
    {
        DirectionLightUniform &dlu = p.second.uniform;
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

void MainNode::_setModelMatrix(const glm::mat4x4 *transformation)
{
    this->_shader.setMat4("model", *transformation);
}

void MainNode::_setCurrentMaterial(const PBRMaterial *material)
{
    this->_shader.setVector3("pbrMaterial.albedo_value", material->albedo_value);
    this->_loadTextureToShader("pbrMaterial.albedo_texture", this->_materialTextureOffset + 0,
                               material->albedo_texture ? *material->albedo_texture : *TextureManager::white.get());

    this->_loadTextureToShader("pbrMaterial.normal_map", this->_materialTextureOffset + 1,
                               material->normal_map ? *material->normal_map : *TextureManager::blue.get());

    this->_shader.setFloat("pbrMaterial.metalness_value", material->metalness_value);
    this->_loadTextureToShader("pbrMaterial.metalness_texture", this->_materialTextureOffset + 2,
                               material->metalness_texture ? *material->metalness_texture : *TextureManager::black.get());

    this->_shader.setFloat("pbrMaterial.roughness_value", material->roughness_value);
    this->_loadTextureToShader("pbrMaterial.roughness_texture", this->_materialTextureOffset + 3,
                               material->roughness_texture ? *material->roughness_texture : *TextureManager::white.get());

    this->_loadTextureToShader("pbrMaterial.ao_map", this->_materialTextureOffset + 4,
                               material->ao_map ? *material->ao_map : *TextureManager::white.get());

    this->_loadTextureToShader("pbrMaterial.parallax_map", this->_materialTextureOffset + 5,
                               material->parallax_map ? *material->parallax_map : *TextureManager::black.get());
}

void MainNode::_setCurrentMaterial(const Material *material)
{
    this->_shader.setVector3("material.diffuse_value", material->diffuse_value);
    this->_loadTextureToShader("material.diffuse_texture", this->_materialTextureOffset + 0,
                               material->diffuse_texture ? *material->diffuse_texture : *TextureManager::white.get());

    this->_shader.setVector3("material.specular_value", material->specular_value);
    this->_shader.setFloat("material.shininess", material->shininess);
    this->_loadTextureToShader("material.specular_texture", this->_materialTextureOffset + 1,
                               material->specular_texture ? *material->specular_texture : *TextureManager::white.get());

    this->_loadTextureToShader("material.reflection_map", this->_materialTextureOffset + 2,
                               material->reflection_map ? *material->reflection_map : *TextureManager::black.get());
    this->_loadTextureToShader("material.normal_map", this->_materialTextureOffset + 3,
                               material->normal_map ? *material->normal_map : *TextureManager::blue.get());
    this->_loadTextureToShader("material.parallax_map", this->_materialTextureOffset + 4,
                               material->parallax_map ? *material->parallax_map : *TextureManager::black.get());
    this->_shader.setVector3("material.emissive_value", material->emissive_value);
}

void MainNode::_loadShader()
{
    RenderNode::_loadShader();

    this->_shader.use();
    this->_shader.setMat4("view", this->_camera.getViewMatrix());
    this->_shader.setMat4("projection", glm::perspective(this->_camera.getZoom(), (float)1620 / (float)1080, 0.1f, 100.0f));
    this->_shader.setFloat("far_plane", PointLightWrapper::far);

    int matNb = 0;
    for (auto &p : this->_sceneContext.dLights)
    {
        std::stringstream ss;
        ss << matNb;
        this->_shader.setMat4(("lightSpaceMatrix" + ss.str()).c_str(), p.second.projection);
    }

    this->_loadLightsToShader();
    this->_setModelMatrix();
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
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void MainNode::setHdr(bool value)
{
    this->_hdr = value;
}

void MainNode::setLabel(std::string label)
{
    this->_label = label;
}

} // namespace leo