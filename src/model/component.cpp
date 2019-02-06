#include "component.hpp"

#include <model/entity.hpp>

namespace leo {
  namespace model {

    Component::Component()
      : RegisteredObject()
    {
      this->_notify(controller::Event::COMPONENT_CREATED);
    }

    const Entity *Component::getEntity() {
      return this->_entity;
    }

    void Component::setEntity(const Entity *entity) {
      this->_notify(controller::Event::COMPONENT_UPDATED);
      this->_entity = entity;
    }

  }  // namespace leo
}  // namespace leo
