#pragma once

#include <renderer/light-uniforms.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/shadow-mapping-node.hpp>
#include <renderer/cube-shadow-map-node.hpp>
#include <model/cube-map.hpp>

namespace leo
{

typedef struct PointLightWrapper
{
    static const float near;
    static const float far;
    Framebuffer &map;
    std::vector<glm::mat4> shadowTransforms;
    PointLightUniform uniform;
    CubeShadowMapNode renderNode;

    PointLightWrapper(PointLightUniform uniform, CubeShadowMapNode renderNode);

} PointLightWrapper;

typedef struct DirectionLightWrapper
{
    Framebuffer map;
    glm::mat4x4 projection;
    DirectionLightUniform uniform;
    ShadowMappingNode renderNode;

    DirectionLightWrapper(
        Framebuffer map, glm::mat4x4 projection, DirectionLightUniform uniform, ShadowMappingNode renderNode);

} DirectionLightWrapper;

} // namespace leo