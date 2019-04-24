#include "entity.hpp"

#include <model/icomponent.hpp>
#include <model/scene-graph.hpp>
#include <model/components/point-light.hpp>
#include <model/components/direction-light.hpp>
#include <controller/event.hpp>

namespace leo
{

t_id Entity::_count = 0;

Entity::Entity()
    : RegisteredObject(_count++)
{
  this->_notify(Event::BASE_CREATED);
}

const std::map<t_typeId, IComponent *> &Entity::getComponents() const
{
  return this->_components;
}

bool Entity::addComponent(IComponent *component)
{
  component->setEntity(this);
  bool result = this->_components.insert(
                                     std::pair<t_typeId, IComponent *>(component->getTypeId(), component))
                    .second;
  this->_notify(*component, Event::COMPONENT_ADDED);
  return result;
}

bool Entity::addComponent(PointLight *component)
{
  return this->addComponent(static_cast<IComponent *>(component));
}

bool Entity::addComponent(DirectionLight *component)
{
  return this->addComponent(static_cast<IComponent *>(component));
}

const IComponent *Entity::getComponent(t_typeId type) const
{
  auto it = this->_components.find(type);
  if (it == this->_components.end())
    return nullptr;
  return it->second;
}

const std::map<t_id, Entity *> &Entity::getChildren() const
{
  return this->_children;
}

bool Entity::addChild(Entity *child)
{
  bool success;
  if (success = this->_children.insert(
                                   std::pair<t_id, Entity *>(child->getId(), child))
                    .second)
  {
    child->setParent(this);
    child->_setSceneGraphRec(this->_sceneGraph);
  }
  for (Observer *obs : this->_observers)
    child->watch(obs);
  child->_notify(Event::BASE_ADDED);
  return success;
}

const Entity *Entity::getParent() const
{
  return this->_parent;
}

void Entity::setParent(Entity *parent)
{
  // NOTE: Probably unsafe due to observer.
  this->_parent = parent;
  this->_notify(Event::COMPONENT_UPDATED);
}

const SceneGraph *Entity::getSceneGraph() const
{
  return this->_sceneGraph;
}

void Entity::setSceneGraph(SceneGraph *sceneGraph)
{
  this->_setSceneGraphRec(sceneGraph);
  this->_notify(Event::COMPONENT_UPDATED);
}

void Entity::_setSceneGraphRec(SceneGraph *sceneGraph)
{
  this->_sceneGraph = sceneGraph;
  for (auto it : this->_children)
    it.second->_setSceneGraphRec(sceneGraph);
}

void Entity::watch(Observer *observer)
{
  Subject::watch(observer);
  for (auto &p : this->_components)
  {
    p.second->watch(observer);
  }
  for (auto &p : this->_children)
  {
    p.second->watch(observer);
  }
}

} // namespace leo
