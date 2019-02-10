#include "icomponent.hpp"

#include <model/entity.hpp>

namespace leo {
  namespace model {

    t_id IComponent::_count = 0;

    IComponent::IComponent()
      : RegisteredObject(_count++)
    {
      this->_notify(controller::Event::COMPONENT_CREATED);
    }

    const Entity *IComponent::getEntity() {
      return this->_entity;
    }

    void IComponent::setEntity(const Entity *entity) {
      this->_notify(controller::Event::COMPONENT_UPDATED);
      this->_entity = entity;
    }

  }  // namespace leo
}  // namespace leo
