#include "component.hpp"

#include <model/entity.hpp>

namespace leo {
  namespace model {

    t_id Component::_count = 0;

    Component::Component()
      : RegisteredObject(_count++)
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
