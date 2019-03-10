#include "registered-object.hpp"

namespace leo {

    RegisteredObject::RegisteredObject(t_id id) : _id(id)
    {
    }

    const t_id &RegisteredObject::getId() const {
      return this->_id;
    }


}  // namespace leo
