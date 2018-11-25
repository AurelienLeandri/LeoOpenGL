#include "object-register.hpp"

namespace leo {
  namespace model {

    unsigned int ObjectRegister::_nbObjects = 0;

    ObjectRegister::ObjectRegister() {
    }

    ObjectRegister &ObjectRegister::getInstance() {
      static std::shared_ptr<ObjectRegister> r(new ObjectRegister());
      return *r;
    }

    bool ObjectRegister::registerObject(std::shared_ptr<const RegisteredObject> object, stringID id) {
      return this->_register.insert(
          std::pair<std::string, std::weak_ptr<const RegisteredObject>>(id,
            std::weak_ptr<const RegisteredObject>(object))).second;
    }

    bool ObjectRegister::unregisterObject(stringID id) {
      auto it = this->_register.find(id);
      if (it == this->_register.end())
        return false;
      this->_register.erase(it);
      return true;
    }

    std::string ObjectRegister::generateStringID() {
      std::stringstream ss;
      ss << _nbObjects++;
      return ss.str();
    }

  } //namespace model
} //namespace leo
