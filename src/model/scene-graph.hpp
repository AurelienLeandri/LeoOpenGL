#pragma once

#include <model/components/point-light.hpp>
#include <model/components/direction-light.hpp>

#include <map>

namespace leo
{
namespace model
{

class Base;
class PointLight;
class DirectionLight;
class CubeMap;

class SceneGraph
{
  public:
    SceneGraph();

  public:
    const Base *getRoot() const;
    void setRoot(Base *root);

  public:
    void addLight(PointLight *light);
    void addLight(DirectionLight *light);
    void removeLight(PointLight *light);
    void removeLight(DirectionLight *light);
    const std::map<stringID, PointLight *> &getPointLights() const;
    const std::map<stringID, DirectionLight *> &getDirectionLights() const;
    const CubeMap *getCubeMap() const;
    void setCubeMap(CubeMap *cubeMap);

  private:
    std::map<stringID, PointLight *> _pointLights;
    std::map<stringID, DirectionLight *> _directionLights;
    CubeMap *_cubeMap = 0;

  private:
    Base *_root = 0;
};
} // namespace model
} // namespace leo