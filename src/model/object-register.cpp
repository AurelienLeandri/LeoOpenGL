#include "object-register.hpp"

#include <sstream>
#include <memory>

namespace leo {
  namespace model {

    unsigned int ObjectRegister::_nbObjects = 0;

    ObjectRegister::ObjectRegister() {
    }

    ObjectRegister &ObjectRegister::getInstance() {
      static std::shared_ptr<ObjectRegister> r(new ObjectRegister());
      return *r;
    }

    bool ObjectRegister::registerObject(const RegisteredObject *object, t_id id) {
      return this->_register.insert(
          std::pair<std::string, const RegisteredObject*>(id, object)).second;
    }

    bool ObjectRegister::unregisterObject(t_id id) {
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
