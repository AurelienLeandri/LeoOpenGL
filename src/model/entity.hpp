#pragma once

#include <controller/subject.hpp>
#include <model/registered-object.hpp>

#include <map>
#include <vector>
#include <memory>

namespace leo
{
namespace model
{

class SceneGraph;
class Component;
class PointLight;
class DirectionLight;

class Entity : public RegisteredObject, public controller::Subject
{

public:
  Entity();

public:
  bool addComponent(std::string id, Component *component);
  bool addComponent(std::string id, DirectionLight *component);
  bool addComponent(std::string id, PointLight *component);
  Component *getComponent(std::string name);
  const std::map<std::string, Component *> &getComponents() const;
  const std::map<t_id, Entity *> &getChildren() const;
  bool addChild(Entity *child);
  const Entity *getParent() const;
  void setParent(Entity *parent);
  const SceneGraph *getSceneGraph() const;
  void setSceneGraph(SceneGraph *sceneGraph);

private:
  void _setSceneGraphRec(SceneGraph *sceneGraph);
  static t_id _count;

private:
  std::map<std::string, Component *> _components;
  std::map<t_id, Entity *> _children;
  Entity *_parent = nullptr;
  SceneGraph *_sceneGraph = nullptr;

}; // class Entity

} // namespace model
} // namespace leo
