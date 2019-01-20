#include "light-uniforms.hpp"
#include <model/components/direction-light.hpp>
#include <model/components/point-light.hpp>

namespace leo
{
namespace renderer
{

// LightUniform

LightUniform::LightUniform() {}

LightUniform::LightUniform(const model::Light &light)
{
    this->ambient = light._ambient;
    this->constant = light._constant;
    this->diffuse = light._diffuse;
    this->linear = light._linear;
    this->specular = light._specular;
    this->quadratic = light._quadratic;
}

LightUniform::~LightUniform() {}

LightUniform::LightUniform(const LightUniform &other) : ambient(other.ambient),
                                                        constant(other.constant),
                                                        diffuse(other.diffuse),
                                                        linear(other.linear),
                                                        specular(other.specular),
                                                        quadratic(other.quadratic)
{
}

LightUniform &LightUniform::operator=(const LightUniform &other)
{
    this->ambient = other.ambient;
    this->constant = other.constant;
    this->diffuse = other.diffuse;
    this->linear = other.linear;
    this->specular = other.specular;
    this->quadratic = other.quadratic;
    return *this;
}

// DirectionLightUniform

DirectionLightUniform::DirectionLightUniform() {}

DirectionLightUniform::DirectionLightUniform(const model::DirectionLight &light) : LightUniform(light)
{
    this->direction = light._direction;
}

DirectionLightUniform::~DirectionLightUniform() {}

DirectionLightUniform::DirectionLightUniform(const DirectionLightUniform &other) : LightUniform::LightUniform(other)
{
    this->direction = other.direction;
}

DirectionLightUniform &DirectionLightUniform::operator=(const DirectionLightUniform &other)
{
    LightUniform::operator=(other);
    this->direction = other.direction;
    return *this;
}

// PointLightUniform

PointLightUniform::PointLightUniform() {}

PointLightUniform::PointLightUniform(const model::PointLight &light) : LightUniform(light)
{
    this->position = light._position;
}

PointLightUniform::~PointLightUniform() {}

PointLightUniform::PointLightUniform(const PointLightUniform &other) : LightUniform::LightUniform(other)
{
    this->position = other.position;
}

PointLightUniform &PointLightUniform::operator=(const PointLightUniform &other)
{
    LightUniform::operator=(other);
    this->position = other.position;
    return *this;
}

} // namespace renderer
} // namespace leo