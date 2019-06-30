#include <renderer/scene-context.hpp>

#include <renderer/light-uniforms.hpp>
#include <renderer/light-wrapper.hpp>
#include <renderer/buffer-collection.hpp>
#include <renderer/texture-wrapper.hpp>
#include <renderer/ibl-wrapper.hpp>
#include <renderer/ibl-node.hpp>
#include <renderer/opengl-context.hpp>

#include <model/components/direction-light.hpp>
#include <model/components/point-light.hpp>
#include <model/components/transformation.hpp>
#include <model/components/material.hpp>
#include <model/components/pbr-material.hpp>
#include <model/components/volume.hpp>
#include <model/entity.hpp>
#include <model/texture-manager.hpp>

#include <utils/texture.hpp>

namespace leo
{

SceneContext::SceneContext(OpenGLContext &context) : _context(context)
{
}

void SceneContext::registerDirectionLight(const DirectionLight &dl, const SceneGraph &sceneGraph, Shader &shadowShader)
{
    auto it = this->dLights.find(dl.getId());
    if (it != this->dLights.end())
        return;

    float near_plane = 1.0f, far_plane = 100.f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::vec3 front = dl.direction;
    glm::vec3 pos = -front * ((near_plane + far_plane) * 0.5f);
    glm::vec3 up(0.f, 0.f, 0.f);
    int index = 0;
    for (index; index < 3; index++)
    {
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

    DepthBufferOptions options;
    options.width = 1620 * 2;
    options.height = 1080 * 2;
    options.type = BufferType::DEPTH_MAP;

    DirectionLightWrapper &wrapper = this->dLights.insert(std::pair<t_id, DirectionLightWrapper>(
                                                              dl.getId(),
                                                              DirectionLightWrapper(Framebuffer(), lightSpaceMatrix, DirectionLightUniform(dl),
                                                                                    ShadowMappingNode(this->_context, *this, sceneGraph, shadowShader, dl))))
                                         .first->second;

    wrapper.map.setDepthBuffer(options);
    wrapper.renderNode.setFramebuffer(&wrapper.map);
    wrapper.renderNode.setLightSpaceMatrix(wrapper.projection);
}

void SceneContext::registerPointLight(const PointLight &pl, const SceneGraph &sceneGraph, Shader &shadowShader)
{
    PointLightUniform plu(pl);
    const Transformation *transform = static_cast<const Transformation *>(pl.getEntity()->getComponent(ComponentType::TRANSFORMATION));

    if (transform)
    {
        const glm::mat4x4 &transformation = transform->getTransformationMatrix();
        plu.position = transformation * pl.position;
    }
    PointLightWrapper &plw = this->pLights.insert(std::pair<t_id, PointLightWrapper>(pl.getId(),
                                                                                     PointLightWrapper(plu, CubeShadowMapNode(this->_context, *this, sceneGraph, shadowShader, pl))))
                                 .first->second;
}

void SceneContext::registerMaterial(const Material &m)
{
    GLTextureOptions options;
    for (const Texture *t : {m.diffuse_texture, m.specular_texture, m.reflection_map, m.normal_map, m.parallax_map})
        if (t)
        {
            if (t->mode == RGBA || t->mode == SRGBA)
            {
                options.format = options.internalFormat = GL_RGBA;
            }
            else
            {
                options.format = options.internalFormat = GL_RGB;
            }
            options.type = GL_UNSIGNED_BYTE;
            this->registerTexture(*t, options, {});
        }
}

void SceneContext::registerMaterial(const PBRMaterial &m)
{
    GLTextureOptions options;
    for (const Texture *t : {m.albedo_texture, m.metalness_texture, m.roughness_texture, m.normal_map, m.ao_map, m.parallax_map})
        if (t)
        {
            if (t->mode == RGBA || t->mode == SRGBA)
            {
                options.format = options.internalFormat = GL_RGBA;
            }
            else
            {
                options.format = options.internalFormat = GL_RGB;
            }
            options.type = GL_UNSIGNED_BYTE;
            this->registerTexture(*t, options, {});
        }
}

void SceneContext::registerTexture(const Texture &tex, GLTextureOptions glOptions = {}, TextureOptions textureOptions = {})
{
    this->textures.insert(std::pair<t_id, TextureWrapper>(tex.getId(), TextureWrapper(tex, glOptions, textureOptions)));
}

void SceneContext::registerIBL(const IBL &ibl, const SceneGraph &sceneGraph, Shader &hdrShader)
{
    this->ibls.insert(std::pair<t_id, IBLWrapper>(ibl.getId(), IBLWrapper(ibl, IBLNode(this->_context, *this, sceneGraph, hdrShader, ibl))));
    this->ibls.find(ibl.getId())->second.node.render();
}

void SceneContext::registerVolume(const Volume &volume)
{
    this->bufferCollectionsInstanced.erase(volume.getId());

    auto it = this->bufferCollections.find(volume.getId());

    if (it == this->bufferCollections.end())
    {
        this->bufferCollections.insert(std::pair<t_id, BufferCollection>(volume.getId(), BufferCollection())).first;
        BufferCollection &bc = this->bufferCollections[volume.getId()];

        this->_context.generateBufferCollection(bc, volume);
    }
}

void SceneContext::registerInstancedVolume(const Volume &volume)
{
    auto it = this->bufferCollectionsInstanced.find(volume.getId());

    if (it == this->bufferCollectionsInstanced.end())
    {

        auto it2 = this->bufferCollections.find(volume.getId());
        if (it2 != this->bufferCollections.end())
        {
            this->bufferCollectionsInstanced.insert(
                std::pair<t_id, BufferCollection>(
                    volume.getId(), it2->second));
            this->bufferCollections.erase(volume.getId());
        }
        else
        {
            this->bufferCollectionsInstanced.insert(
                std::pair<t_id, BufferCollection>(
                    volume.getId(), BufferCollection()));
        }
        BufferCollection &bc = this->bufferCollectionsInstanced[volume.getId()];
        this->_context.generateBufferCollectionInstanced(bc, volume, this->instancingVBO);
    }
}

void SceneContext::setInstancingVBO(const std::vector<glm::mat4> &transformations)
{
    this->instancingVBO = this->_context.generateInstancingVBO(transformations);
}

} // namespace leo