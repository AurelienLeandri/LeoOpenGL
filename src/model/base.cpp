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

  } // namespace leo
} // namespace model
