#pragma once

#include <renderer/global.hpp>

namespace leo
{

class Light
{
public:
  Light();
  Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
  Light(float constant, float linear, float quadratic,
        glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
  virtual ~Light();

public:
  float constant = 1.f;
  float linear = 0.09f;
  float quadratic = 0.032;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};

} // namespace leo
