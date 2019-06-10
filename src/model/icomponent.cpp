#include "icomponent.hpp"

#include <model/entity.hpp>

namespace leo
{

IComponent::IComponent()
    : RegisteredObject()
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
