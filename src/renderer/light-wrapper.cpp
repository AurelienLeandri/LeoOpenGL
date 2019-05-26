#include <renderer/light-wrapper.hpp>

namespace leo
{

const float PointLightWrapper::near = 0.01f;
const float PointLightWrapper::far = 25.f;

PointLightWrapper::PointLightWrapper(PointLightUniform uniform, CubeShadowMapNode renderNode)
    : map(*renderNode.getOutput()), uniform(uniform), renderNode(renderNode)
{
    float aspect = 1024.f / 1024.f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
    glm::vec3 lightPos = this->uniform.position;
    shadowTransforms.push_back(shadowProj *
                               glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(shadowProj *
                               glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(shadowProj *
                               glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    shadowTransforms.push_back(shadowProj *
                               glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
    shadowTransforms.push_back(shadowProj *
                               glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(shadowProj *
                               glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
}

DirectionLightWrapper::DirectionLightWrapper(
    Framebuffer map, glm::mat4x4 projection, DirectionLightUniform uniform, ShadowMappingNode renderNode)
    : map(map), projection(projection), uniform(uniform), renderNode(renderNode)
{
}

} // namespace leo