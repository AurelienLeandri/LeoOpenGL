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

class Base : public RegisteredObject, public controller::Subject
{

public:
  Base();

public:
  bool addComponent(std::string id, Component *component);
  bool addComponent(std::string id, DirectionLight *component);
  bool addComponent(std::string id, PointLight *component);
  Component *getComponent(std::string name);
  const std::map<std::string, Component *> &getComponents() const;
  const std::map<stringID, Base *> &getChildren() const;
  bool addChild(Base *child);
  const Base *getParent() const;
  void setParent(Base *parent);
  const SceneGraph *getSceneGraph() const;
  void setSceneGraph(SceneGraph *sceneGraph);

private:
  void _setSceneGraphRec(SceneGraph *sceneGraph);

private:
  std::map<std::string, Component *> _components;
  std::map<stringID, Base *> _children;
  Base *_parent = nullptr;
  SceneGraph *_sceneGraph = nullptr;

}; // class Base

} // namespace model
} // namespace leo
