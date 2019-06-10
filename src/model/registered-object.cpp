#include "registered-object.hpp"

namespace leo
{

t_id RegisteredObject::current_id = 0;

RegisteredObject::RegisteredObject() : _id(current_id++)
{
}

const t_id &RegisteredObject::getId() const
{
  return this->_id;
}

} // namespace leo
