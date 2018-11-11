#include "component.hpp"

namespace leo {
  namespace model {

    Component::Component()
      : RegisteredObject("component")
    {
    }

    Component::Component(std::vector<std::string> prerequisites)
      : Component()
    {
      for (auto &s : prerequisites) {
        this->_linkedComponents.insert(
            std::pair<std::string, std::shared_ptr<Component>>(s, nullptr));
      }
    }

    std::map<std::string, std::shared_ptr<Component>> Component::getLinkedComponents() {
      return this->_linkedComponents;
    }

    bool Component::addLinkedComponent(std::string name, Component *component) {
      return this->_linkedComponents.insert(
          std::pair<std::string, std::shared_ptr<Component>>(name, std::shared_ptr<Component>(component))).second;
    }

    std::shared_ptr<Component> Component::getLinkedComponent(std::string name) {
      auto it = this->_linkedComponents.find(name);
      if (it == this->_linkedComponents.end())
        return nullptr;
      return it->second;
    }

    std::map<stringID, std::shared_ptr<const Base>> Component::getParentBases() {
      return this->_parentBases;
    }

    std::shared_ptr<const Base> Component::getParentBase(stringID id) {
      auto it = this->_parentBases.find(id);
      if (it == this->_parentBases.end())
        return nullptr;
      return it->second;
    }

    bool Component::addParentBase(const Base *base) {
      return this->_parentBases.insert(
          std::pair<stringID, std::shared_ptr<const Base>>(base->getId(), std::shared_ptr<const Base>(base))).second;
    }

  }  // namespace leo
}  // namespace leo
