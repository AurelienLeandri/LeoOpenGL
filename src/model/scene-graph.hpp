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
  virtual void watch(Observer *observer) override;

public:
  const Entity *getRoot() const;
  void setRoot(Entity *root);

public:
  const CubeMap *getCubeMap() const;
  void setCubeMap(CubeMap *cubeMap);

private:
  std::map<t_id, PointLight *> _pointLights;
  std::map<t_id, DirectionLight *> _directionLights;
  CubeMap *_cubeMap = 0;

private:
  Entity *_root = 0;
};

} // namespace leo