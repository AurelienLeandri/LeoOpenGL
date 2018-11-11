#pragma once

#include <map>
#include <vector>
#include <memory>
#include <exception>
#include <model/registered-object.hpp>
#include <model/component.hpp>

namespace leo {
  namespace model {

    class Component;

    class Base : public RegisteredObject {

      public:
        Base();
        virtual ~Base();
        Base(const Base &other);
        const Base &operator=(const Base &other);

      public:
        bool addComponent(std::string id, Component *component);
        std::shared_ptr<Component> getComponent(std::string name);
        std::map<std::string, std::shared_ptr<Component>> &getComponents();

      private:
        std::map<std::string, std::shared_ptr<Component>> _components;

    }; // class Base


  } // namespace leo
} // namespace model
