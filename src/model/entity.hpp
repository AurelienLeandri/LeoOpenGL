#pragma once

#include <controller/subject.hpp>
#include <model/registered-object.hpp>

#include <map>
#include <vector>
#include <memory>

namespace leo
{

class SceneGraph;
class IComponent;
class PointLight;
class DirectionLight;

using t_typeId = unsigned int;

class Entity : public RegisteredObject, public Subject
{

public:
  Entity();

public:
  bool addComponent(IComponent *component);
  bool addComponent(DirectionLight *component);
  bool addComponent(PointLight *component);
  const IComponent *getComponent(t_typeId type) const;
  const std::map<t_typeId, IComponent *> &getComponents() const;
  const std::map<t_id, Entity *> &getChildren() const;
  bool addChild(Entity *child);
  const Entity *getParent() const;
  void setParent(Entity *parent);
  const SceneGraph *getSceneGraph() const;
  void setSceneGraph(SceneGraph *sceneGraph);
  void reloadScene(std::vector<Observer *> observers);

private:
  void _setSceneGraphRec(SceneGraph *sceneGraph);
  static t_id _count;

private:
  std::map<t_typeId, IComponent *> _components;
  std::map<t_id, Entity *> _children;
  Entity *_parent = nullptr;
  SceneGraph *_sceneGraph = nullptr;

}; // class Entity

} // namespace leo
