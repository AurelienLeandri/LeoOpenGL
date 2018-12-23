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
        Component *getComponent(std::string name);
        const std::map<std::string, Component*> &getComponents() const;
        const std::map<stringID, Base*> &getChildren() const;
        bool addChild(Base *child);
        const Base *getParent() const;
        void setParent(Base *parent);
        const Base *getRoot() const;
        void setRoot(Base *root);

      private:
        void _setRootRec(Base *root);

      private:
        std::map<std::string, Component*> _components;
        std::map<stringID, Base*> _children;
        Base *_parent;
        Base *_root;

    }; // class Base


  } // namespace leo
} // namespace model
