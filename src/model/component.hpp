#pragma once

#include <model/registered-object.hpp>
#include <controller/subject.hpp>

#include <map>
#include <vector>
#include <memory>

namespace leo {
  namespace model {

    using typeId = unsigned int;
    using t_componentId = unsigned int;
    using t_entityId = unsigned int;
    using t_textureId = unsigned int;

    class Entity;

    class Component : public RegisteredObject, public controller::Subject {
      public:
        Component();
        virtual ~Component() = default;

      public:
        std::map<stringID, const Entity*> getParentEntities();
        const Entity *getEntity();
        void setEntity(const Entity *entity);

      protected:
        const Entity *_entity = nullptr;
        
    };  // class Component

  }  // namespace model
}  // namespace leo
