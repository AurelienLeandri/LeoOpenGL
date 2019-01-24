#pragma once

#include <renderer/global.hpp>

namespace leo
{

namespace model
{
class Light;
class DirectionLight;
class PointLight;
} // namespace model

namespace renderer
{

typedef struct LightUniform
{
    LightUniform();
    LightUniform(const model::Light &light);
    ~LightUniform();
    LightUniform(const LightUniform &other);
    LightUniform &operator=(const LightUniform &other);
    float constant;
    glm::vec3 diffuse;
    float linear;
    glm::vec3 specular;
    float quadratic;
} LightUniform;

typedef struct DirectionLightUniform : public LightUniform
{
    DirectionLightUniform();
    DirectionLightUniform(const model::DirectionLight &light);
    ~DirectionLightUniform();
    DirectionLightUniform(const DirectionLightUniform &other);
    DirectionLightUniform &operator=(const DirectionLightUniform &other);
    glm::vec4 direction;
} DirectionLightUniform;

typedef struct PointLightUniform : public LightUniform
{
    PointLightUniform();
    PointLightUniform(const model::PointLight &light);
    ~PointLightUniform();
    PointLightUniform(const PointLightUniform &other);
    PointLightUniform &operator=(const PointLightUniform &other);
    glm::vec4 position;
} PointLightUniform;

} // namespace renderer
} // namespace leo