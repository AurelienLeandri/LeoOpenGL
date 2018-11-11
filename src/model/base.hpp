#pragma once

#include <map>
#include <vector>
#include <memory>
#include <exception>

namespace leo {
  namespace model {

    class Base : public RegisteredObject {
      using std::pair<std::string, std::shared_ptr<Component>> = map_entry;

      public:
        Base();
        virtual ~Base();
        Base(const Base &other);
        const Base &operator=(const Base &other);

      public:
        bool addComponent(std::string id, std::shared_ptr<Component> component);
        std::shared_ptr<Component> getComponent(std::string name);
        std::map<std::string, std::shared_ptr<Component>> &getComponents();

      private:
        std::map<std::string, std::shared_ptr<Component>> _components;

    }; // class Base


  } // namespace leo
} // namespace model
