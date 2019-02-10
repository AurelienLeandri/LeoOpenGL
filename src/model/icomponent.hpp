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

    class IComponent : public RegisteredObject, public controller::Subject {
      public:
        IComponent();
        virtual ~IComponent() = default;

      public:
        std::map<t_id, const Entity*> getParentEntities();
        const Entity *getEntity();
        void setEntity(const Entity *entity);
        virtual t_id getTypeId() = 0;

      protected:
        const Entity *_entity = nullptr;

      private:
        static t_id _count;
        
    };  // class Component

  }  // namespace model
}  // namespace leo
