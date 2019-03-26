#include "material.hpp"

namespace leo
{

Material::Material() : Material(false)
{
}

Material::Material(bool force) : force(force), diffuse_value(1.f, 1.f, 1.f), specular_value(1.f, 1.f, 1.f)
{
}

} // namespace leo
