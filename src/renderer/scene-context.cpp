#include <renderer/scene-context.hpp>

#include <renderer/light-uniforms.hpp>
#include <renderer/light-wrapper.hpp>
#include <renderer/buffer-collection.hpp>
#include <renderer/texture-wrapper.hpp>

#include <model/components/direction-light.hpp>
#include <model/components/point-light.hpp>
#include <model/components/transformation.hpp>
#include <model/entity.hpp>

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

    FramebufferOptions options;
    options.width = 1620 * 2;
    options.height = 1080 * 2;
    options.type = FrameBufferType::DEPTH_MAP;

    DirectionLightWrapper &wrapper = this->dLights.insert(std::pair<t_id, DirectionLightWrapper>(
                                                              dl.getId(),
                                                              DirectionLightWrapper(Framebuffer(options), lightSpaceMatrix, DirectionLightUniform(dl),
                                                                                    ShadowMappingNode(this->_context, sceneGraph, shadowShader, dl))))
                                         .first->second;

    wrapper.map.generate();
    wrapper.renderNode.setOutput(&wrapper.map);
    wrapper.renderNode.setLightSpaceMatrix(wrapper.projection);
}

void SceneContext::registerPointLight(const PointLight &pl)
{
    PointLightUniform &plu = this->pLights.insert(std::pair<t_id, PointLightWrapper>(pl.getId(), PointLightWrapper(pl)))
                                 .first->second.uniform;
    const Transformation *transform = static_cast<const Transformation *>(pl.getEntity()->getComponent(ComponentType::TRANSFORMATION));
    if (transform)
    {
      const glm::mat4x4 &transformation = transform->getTransformationMatrix();
      plu.position = transformation * pl.position;
    }
}

} // namespace leo