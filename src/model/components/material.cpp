#include "material.hpp"

namespace leo
{

Material::Material() : Material(false)
{
}

Material::Material(bool force) : force(force), diffuse_value(0.f, 0.f, 0.f), specular_value(0.f, 0.f, 0.f)
{
}

} // namespace leo
