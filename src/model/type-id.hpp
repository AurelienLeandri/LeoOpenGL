#pragma once

#include <iostream>

namespace leo
{
namespace model
{

enum ComponentType {
  DIRECTION_LIGHT,
  MATERIAL,
  POINT_LIGHT,
  TRANSFORMATION,
  VOLUME,
  INSTANCED
};

} // namespace model
} // namespace leo