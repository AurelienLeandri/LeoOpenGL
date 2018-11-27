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
        const std::map<std::string, std::shared_ptr<Component>> &getComponents() const;
        const std::map<stringID, std::shared_ptr<Base>> &getChildren() const;
        bool addChild(Base *child);
        std::weak_ptr<const Base> getParent() const;
        void setParent(Base *parent);
        std::weak_ptr<const Base> getRoot() const;
        void setRoot(Base *root);

      private:
        void _setRootRec(Base *root);

      private:
        std::map<std::string, std::shared_ptr<Component>> _components;
        std::map<stringID, std::shared_ptr<Base>> _children;
        std::weak_ptr<Base> _parent;
        std::weak_ptr<Base> _root;

    }; // class Base


  } // namespace leo
} // namespace model
