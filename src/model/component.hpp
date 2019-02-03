#pragma once

#include <model/registered-object.hpp>
#include <controller/subject.hpp>
#include <map>
#include <vector>
#include <memory>

namespace leo {
  namespace model {

    class Base;

    class Component : public RegisteredObject, public controller::Subject {
      public:
        Component();
        virtual ~Component() = default;

      public:
        std::map<stringID, const Base*> getParentBases();
        const Base *getBase();
        void setBase(const Base *base);

      protected:
        const Base *_base = nullptr;
        
    };  // class Component

  }  // namespace model
}  // namespace leo
