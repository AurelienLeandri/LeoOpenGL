#pragma once

#include <renderer/global.hpp>

namespace leo
{

class Light;
class DirectionLight;
class PointLight;

typedef struct LightUniform
{
    LightUniform();
    LightUniform(const Light &light);
    ~LightUniform();
    LightUniform(const LightUniform &other);
    LightUniform &operator=(const LightUniform &other);
    glm::vec3 ambient;
    float constant = 1.f;
    glm::vec3 diffuse;
    float linear = 0.9f;
    glm::vec3 specular;
    float quadratic = 0.032f;
} LightUniform;

typedef struct DirectionLightUniform : public LightUniform
{
    DirectionLightUniform();
    DirectionLightUniform(const DirectionLight &light);
    ~DirectionLightUniform();
    DirectionLightUniform(const DirectionLightUniform &other);
    DirectionLightUniform &operator=(const DirectionLightUniform &other);
    glm::vec4 direction;
} DirectionLightUniform;

typedef struct PointLightUniform : public LightUniform
{
    PointLightUniform();
    PointLightUniform(const PointLight &light);
    ~PointLightUniform();
    PointLightUniform(const PointLightUniform &other);
    PointLightUniform &operator=(const PointLightUniform &other);
    glm::vec4 position;
} PointLightUniform;

} // namespace leo