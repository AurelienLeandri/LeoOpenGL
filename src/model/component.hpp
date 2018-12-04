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
        Component( const Component &other);
        const Component &operator=(const Component &other);
        virtual ~Component();

      public:
        std::map<stringID, std::shared_ptr<const Base>> getParentBases();
        std::shared_ptr<const Base> getBase();
        void setBase(const Base *base);

      protected:
        std::shared_ptr<const Base> _base;
        
    };  // class Component

  }  // namespace model
}  // namespace leo
