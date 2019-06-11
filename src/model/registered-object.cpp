#include "registered-object.hpp"

namespace leo
{

t_id RegisteredObject::_currentId = 1;

RegisteredObject::RegisteredObject() : _id(_currentId++)
{
}

const t_id &RegisteredObject::getId() const
{
  return this->_id;
}

} // namespace leo
