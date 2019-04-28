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
    Framebuffer &map;
    glm::mat4x4 projections[6];
    PointLightUniform uniform;
    CubeShadowMapNode renderNode;

    PointLightWrapper(PointLightUniform uniform, CubeShadowMapNode renderNode)
    : map(*renderNode.getOutputs()["out"]), uniform(uniform), renderNode(renderNode)
    {
    }

} PointLightWrapper;

typedef struct DirectionLightWrapper
{
    Framebuffer map;
    glm::mat4x4 projection;
    DirectionLightUniform uniform;
    ShadowMappingNode renderNode;

    DirectionLightWrapper(
        Framebuffer map, glm::mat4x4 projection, DirectionLightUniform uniform, ShadowMappingNode renderNode)
    : map(map), projection(projection), uniform(uniform), renderNode(renderNode)
    {
    }

} DirectionLightWrapper;

} // namespace leo