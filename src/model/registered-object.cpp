#include "registered-object.hpp"

#include <model/object-register.hpp>

namespace leo {
  namespace model {

    RegisteredObject::RegisteredObject(t_id id) : _id(id)
    {
    }

    const t_id &RegisteredObject::getId() const {
      return this->_id;
    }


  }  // namespace leo
}  // namespace model
