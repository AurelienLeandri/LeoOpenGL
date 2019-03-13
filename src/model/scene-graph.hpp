#pragma once

#include <controller/subject.hpp>

#include <map>
#include <vector>

namespace leo
{

class Observer;
class Entity;
class PointLight;
class DirectionLight;
class CubeMap;

class SceneGraph : public Subject
{

  using t_id = unsigned int;

public:
  SceneGraph();

public:
  const Entity *getRoot() const;
  void setRoot(Entity *root);

public:
  void addLight(PointLight *light);
  void addLight(DirectionLight *light);
  void removeLight(PointLight *light);
  void removeLight(DirectionLight *light);
  const std::map<t_id, PointLight *> &getPointLights() const;
  const std::map<t_id, DirectionLight *> &getDirectionLights() const;
  const CubeMap *getCubeMap() const;
  void setCubeMap(CubeMap *cubeMap);
  void reloadScene(std::vector<Observer *> observers);

private:
  std::map<t_id, PointLight *> _pointLights;
  std::map<t_id, DirectionLight *> _directionLights;
  CubeMap *_cubeMap = 0;

private:
  Entity *_root = 0;
};

} // namespace leo