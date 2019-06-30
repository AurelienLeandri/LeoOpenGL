#pragma once

#include <iostream>

namespace leo
{

enum ComponentType
{
  DIRECTION_LIGHT,
  MATERIAL,
  POINT_LIGHT,
  TRANSFORMATION,
  VOLUME,
  INSTANCED,
  PBR_MATERIAL,
  IBL_MAP
};

} // namespace leo