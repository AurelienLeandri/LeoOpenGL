#include "pbr-material.hpp"

namespace leo
{

PBRMaterial::PBRMaterial() : PBRMaterial(false)
{
}

PBRMaterial::PBRMaterial(bool force) : force(force), albedo_value(1.f, 1.f, 1.f), metalness_value(0.f), roughness_value(0.f)
{
}

} // namespace leo
