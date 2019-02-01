#include "registered-object.hpp"
#include <model/object-register.hpp>

namespace leo {
  namespace model {

    RegisteredObject::RegisteredObject()
      : _register(ObjectRegister::getInstance())
        , _id(ObjectRegister::generateStringID())
    {
      this->_register.registerObject(this, this->_id);
    }

    const stringID &RegisteredObject::getId() const {
      return this->_id;
    }


  }  // namespace leo
}  // namespace model
