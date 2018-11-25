#include "registered-object.hpp"
#include <model/object-register.hpp>

namespace leo {
  namespace model {

    RegisteredObject::RegisteredObject()
      : _register(ObjectRegister::getInstance())
        , _id(ObjectRegister::generateStringID())
    {
      this->_register.registerObject(std::shared_ptr<RegisteredObject>(this), this->_id);
    }

    RegisteredObject::~RegisteredObject() {
      this->_register.unregisterObject(this->_id);
    }

    RegisteredObject::RegisteredObject(const RegisteredObject &other) :
      _register(ObjectRegister::getInstance())
    {
      this->_id = ObjectRegister::generateStringID();
      this->_register.registerObject(std::shared_ptr<RegisteredObject>(this), this->_id);
    }

    const RegisteredObject &RegisteredObject::operator=(const RegisteredObject &other) {
      this->_register = ObjectRegister::getInstance();
      this->_id = ObjectRegister::generateStringID();
      this->_register.registerObject(std::shared_ptr<RegisteredObject>(this), this->_id);
      return *this;
    }

    const stringID &RegisteredObject::getId() const {
      return this->_id;
    }


  }  // namespace leo
}  // namespace model
