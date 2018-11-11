#include "base.hpp"

namespace leo {
  namespace model {

    Base::Base() 
      : RegisteredObject("base")
    {
    }

    Base::~Base() {
    }

    Base::Base(const Base &other)
      : Base()
    {
      this->_components = other._components;
    }

    const Base &Base::operator=(const Base &other) {
      this->_components = other._components;
      return *this;
    }

    std::map<std::string, std::shared_ptr<Component>> &Base::getComponents() {
      return this->_components;
    }

    bool Base::addComponent(std::string name, Component *component) {
      component->addParentBase(this);
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