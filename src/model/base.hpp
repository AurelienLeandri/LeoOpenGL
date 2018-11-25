#pragma once

#include <controller/subject.hpp>
#include <model/registered-object.hpp>
#include <map>
#include <vector>
#include <memory>

namespace leo {
  namespace model {

    class Component;

    class Base : public RegisteredObject, public controller::Subject {

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
