#include "entity.hpp"

#include <model/component.hpp>
#include <model/scene-graph.hpp>
#include <controller/event.hpp>

namespace leo {
  namespace model {

    Entity::Entity() 
      : RegisteredObject()
    {
      this->_notify(controller::Event::BASE_CREATED);
    }

    const std::map<std::string, Component*> &Entity::getComponents() const {
      return this->_components;
    }

    bool Entity::addComponent(std::string name, Component *component) {
      component->setEntity(this);
      this->_notify(controller::Event::BASE_UPDATED);
      return this->_components.insert(
          std::pair<std::string, Component*>(name, component)).second;
    }

    bool Entity::addComponent(std::string name, PointLight *component) {
      this->_sceneGraph->addLight(component);
      this->addComponent(name, (Component *) component);
    }

    bool Entity::addComponent(std::string name, DirectionLight *component) {
      this->_sceneGraph->addLight(component);
      this->addComponent(name, (Component *) component);
    }

    Component *Entity::getComponent(std::string name) {
      auto it = this->_components.find(name);
      if (it == this->_components.end())
        return nullptr;
      return it->second;
    }

    const std::map<stringID, Entity*> &Entity::getChildren() const {
      return this->_children;
    }

    bool Entity::addChild(Entity *child) {
      bool success;
      if (success = this->_children.insert(
          std::pair<stringID, Entity*>(child->getId(), child)).second)
      {
        child->setParent(this);
        child->_setSceneGraphRec(this->_sceneGraph);
      }
      this->_notify(controller::Event::COMPONENT_UPDATED);
      return success;
    }

    const Entity *Entity::getParent() const {
      return this->_parent;
    }

    void Entity::setParent(Entity *parent) {
      this->_parent = parent;
      this->_notify(controller::Event::COMPONENT_UPDATED);
    }

    const SceneGraph *Entity::getSceneGraph() const {
      return this->_sceneGraph;
    }

    void Entity::setSceneGraph(SceneGraph *sceneGraph) {
      this->_setSceneGraphRec(sceneGraph);
      this->_notify(controller::Event::COMPONENT_UPDATED);
    }

    void Entity::_setSceneGraphRec(SceneGraph *sceneGraph) {
      this->_sceneGraph = sceneGraph;
      for (auto it: this->_children)
        it.second->_setSceneGraphRec(sceneGraph);
    }

  } // namespace leo
} // namespace model
