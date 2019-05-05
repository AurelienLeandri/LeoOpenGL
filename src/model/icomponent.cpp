#include "icomponent.hpp"

#include <model/entity.hpp>

namespace leo
{

t_id IComponent::_count = 1;

IComponent::IComponent()
    : RegisteredObject(_count++)
{
  this->_notify(Event::COMPONENT_CREATED);
}

const Entity *IComponent::getEntity() const
{
  return this->_entity;
}

void IComponent::setEntity(const Entity *entity)
{
  this->_notify(Event::COMPONENT_UPDATED);
  this->_entity = entity;
}

} // namespace leo
