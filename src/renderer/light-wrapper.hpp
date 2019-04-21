#pragma once

#include <renderer/light-uniforms.hpp>
#include <renderer/framebuffer.hpp>
#include <renderer/shadow-mapping-node.hpp>

namespace leo
{

typedef struct LightWrapper
{
    Framebuffer map;
    glm::mat4x4 projection;

    LightWrapper(Framebuffer map, glm::mat4x4 projection) : map(map), projection(projection)
    {
    }
} LightWrapper;

typedef struct PointLightWrapper : public LightWrapper
{
    PointLightUniform uniform;
} PointLightWrapper;

typedef struct DirectionLightWrapper : public LightWrapper
{
    DirectionLightUniform uniform;
    ShadowMappingNode renderNode;

    DirectionLightWrapper(Framebuffer map, glm::mat4x4 projection, DirectionLightUniform uniform, ShadowMappingNode renderNode) : LightWrapper(map, projection), uniform(uniform), renderNode(renderNode)
    {
    }

} DirectionLightWrapper;

} // namespace leo