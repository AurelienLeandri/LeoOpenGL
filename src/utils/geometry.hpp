#pragma once

#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>

namespace leo
{

struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
  glm::vec3 tangent;
  glm::vec3 biTangent;
};

} // namespace leo
