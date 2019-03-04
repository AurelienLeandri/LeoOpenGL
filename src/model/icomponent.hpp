#pragma once

#include <model/registered-object.hpp>
#include <controller/subject.hpp>
#include <model/type-id.hpp>

#include <map>
#include <vector>
#include <memory>

namespace leo {
  namespace model {

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
        const Entity *getEntity() const;
        void setEntity(const Entity *entity);
        virtual ComponentType getTypeId() const = 0;

      protected:
        const Entity *_entity = nullptr;

      private:
        static t_id _count;
        
    };  // class Component

  }  // namespace model
}  // namespace leo
