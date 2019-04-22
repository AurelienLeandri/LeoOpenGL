#pragma once

#include <renderer/light-uniforms.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/shadow-mapping-node.hpp>

namespace leo
{

typedef struct PointLightWrapper
{
    PointLightUniform uniform;

    PointLightWrapper(PointLightUniform uniform) : uniform(uniform)
    {
    }

} PointLightWrapper;

typedef struct DirectionLightWrapper
{
    Framebuffer map;
    glm::mat4x4 projection;
    DirectionLightUniform uniform;
    ShadowMappingNode renderNode;

    DirectionLightWrapper(Framebuffer map, glm::mat4x4 projection, DirectionLightUniform uniform, ShadowMappingNode renderNode) : map(map), projection(projection), uniform(uniform), renderNode(renderNode)
    {
    }

} DirectionLightWrapper;

} // namespace leo