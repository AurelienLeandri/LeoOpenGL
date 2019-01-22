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

class SceneGraph
{
  public:
    SceneGraph();
    ~SceneGraph();
    SceneGraph(const SceneGraph &other) = delete;

  public:
    const SceneGraph &operator=(const SceneGraph &other) = delete;
    const Base *getRoot() const;
    void setRoot(Base *root);

  public:
    void addLight(PointLight *light);
    void addLight(DirectionLight *light);
    void removeLight(PointLight *light);
    void removeLight(DirectionLight *light);
    const std::map<stringID, PointLight *> &getPointLights() const;
    const std::map<stringID, DirectionLight *> &getDirectionLights() const;

  private:
    std::map<stringID, PointLight *> _pointLights;
    std::map<stringID, DirectionLight *> _directionLights;

  private:
    Base *_root;
};
} // namespace model
} // namespace leo