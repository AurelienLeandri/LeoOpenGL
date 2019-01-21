#include "base.hpp"
#include <model/component.hpp>
#include <controller/event.hpp>
#include <model/scene-graph.hpp>
#include <model/components/light.hpp>

namespace leo {
  namespace model {

    Base::Base() 
      : RegisteredObject()
    {
      this->_notify(controller::Event::BASE_CREATED);
    }

    Base::~Base() {
      this->_notify(controller::Event::BASE_DELETED);
    }

    Base::Base(const Base &other)
      : RegisteredObject(other)
    {
      this->_components = other._components;
      this->_notify(controller::Event::BASE_CREATED);
    }

    const Base &Base::operator=(const Base &other) {
      RegisteredObject::operator=(other);
      this->_components = other._components;
      this->_notify(controller::Event::BASE_UPDATED);
      return *this;
    }

    const std::map<std::string, Component*> &Base::getComponents() const {
      return this->_components;
    }

    bool Base::addComponent(std::string name, Component *component) {
      component->setBase(this);
      this->_notify(controller::Event::BASE_UPDATED);
      return this->_components.insert(
          std::pair<std::string, Component*>(name, component)).second;
    }

    bool Base::addComponent(std::string name, PointLight *component) {
      this->_sceneGraph->addLight(component);
      this->addComponent(name, (Component *) component);
    }

    bool Base::addComponent(std::string name, DirectionLight *component) {
      this->_sceneGraph->addLight(component);
      this->addComponent(name, (Component *) component);
    }

    Component *Base::getComponent(std::string name) {
      auto it = this->_components.find(name);
      if (it == this->_components.end())
        return nullptr;
      return it->second;
    }

    const std::map<stringID, Base*> &Base::getChildren() const {
      return this->_children;
    }

    bool Base::addChild(Base *child) {
      bool success;
      if (success = this->_children.insert(
          std::pair<stringID, Base*>(child->getId(), child)).second)
      {
        child->setParent(this);
        child->_setSceneGraphRec(this->_sceneGraph);
      }
      this->_notify(controller::Event::COMPONENT_UPDATED);
      return success;
    }

    const Base *Base::getParent() const {
      return this->_parent;
    }

    void Base::setParent(Base *parent) {
      this->_parent = parent;
      this->_notify(controller::Event::COMPONENT_UPDATED);
    }

    const SceneGraph *Base::getSceneGraph() const {
      return this->_sceneGraph;
    }

    void Base::setSceneGraph(SceneGraph *sceneGraph) {
      this->_setSceneGraphRec(sceneGraph);
      this->_notify(controller::Event::COMPONENT_UPDATED);
    }

    void Base::_setSceneGraphRec(SceneGraph *sceneGraph) {
      this->_sceneGraph = sceneGraph;
      for (auto it: this->_children)
        it.second->_setSceneGraphRec(sceneGraph);
    }

  } // namespace leo
} // namespace model
