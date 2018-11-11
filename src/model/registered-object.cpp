#include "registered-object.hpp"

namespace leo {
  namespace model {

    RegisteredObject::RegisteredObject(std::string prefix)
      : _register(ObjectRegister::getInstance())
        , _id(ObjectRegister::generateStringID(prefix))
    {
      this->_register.registerObject(std::shared_ptr<RegisteredObject>(this), this->_id);
    }

    RegisteredObject::~RegisteredObject() {
      this->_register.unregisterObject(this->_id);
    }

    const stringID &RegisteredObject::getId() const {
      return this->_id;
    }


  }  // namespace leo
}  // namespace model
