#include "base.hpp"
#include <model/component.hpp>
#include <controller/event.hpp>

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

    const std::map<std::string, std::shared_ptr<Component>> &Base::getComponents() const {
      return this->_components;
    }

    bool Base::addComponent(std::string name, Component *component) {
      component->setBase(this);
      this->_notify(controller::Event::BASE_UPDATED);
      return this->_components.insert(
          std::pair<std::string, std::shared_ptr<Component>>(name,
            std::shared_ptr<Component>(component))).second;
    }

    std::shared_ptr<Component> Base::getComponent(std::string name) {
      auto it = this->_components.find(name);
      if (it == this->_components.end())
        return nullptr;
      return it->second;
    }

    const std::map<stringID, std::shared_ptr<Base>> &Base::getChildren() const {
      return this->_children;
    }

    bool Base::addChild(Base *child) {
      bool success;
      if (success = this->_children.insert(
          std::pair<stringID, std::shared_ptr<Base>>(child->getId(),
            std::shared_ptr<Base>(child))).second)
      {
        child->setParent(this);
        child->_setRootRec(this->_root.lock().get());
      }
      this->_notify(controller::Event::COMPONENT_UPDATED);
      return success;
    }

    std::weak_ptr<const Base> Base::getParent() const {
      return this->_parent;
    }

    void Base::setParent(Base *parent) {
      this->_parent = std::shared_ptr<Base>(parent);
      this->_notify(controller::Event::COMPONENT_UPDATED);
    }

    std::weak_ptr<const Base> Base::getRoot() const {
      return this->_root;
    }

    void Base::setRoot(Base *root) {
      this->_setRootRec(root);
      this->_notify(controller::Event::COMPONENT_UPDATED);
    }

    void Base::_setRootRec(Base *root) {
      this->_root = std::shared_ptr<Base>(root);
      for (auto it: this->_children)
        it.second->_setRootRec(root);
    }

  } // namespace leo
} // namespace model
