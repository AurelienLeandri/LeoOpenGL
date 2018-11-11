#pragma once

#include <model/base.hpp>
#include <model/registered-object.hpp>
#include <map>
#include <vector>

namespace leo {
  namespace model {

    class Base;

    class Component : public RegisteredObject {
      public:
        Component();
        Component(std::vector<std::string> prerequisites);

      public:
        std::map<std::string, std::shared_ptr<Component>> getLinkedComponents();
        bool addLinkedComponent(std::string name, Component *component);
        std::shared_ptr<Component> getLinkedComponent(std::string name);
        std::map<stringID, std::shared_ptr<const Base>> getParentBases();
        std::shared_ptr<const Base> getParentBase(stringID id);
        bool addParentBase(const Base *base);

      private:
        std::map<std::string, std::shared_ptr<Component>> _linkedComponents;
        std::map<stringID, std::shared_ptr<const Base>> _parentBases;
        
    };  // class Component

  }  // namespace leo
}  // namespace leo
