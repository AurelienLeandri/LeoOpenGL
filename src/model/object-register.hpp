#pragma once

#include <map>
#include <memory>
#include <string>
#include <assert.h>
#include <sstream>

namespace leo {
  namespace model {

    using stringID = std::string;

    class RegisteredObject;

    class ObjectRegister {

      public:
        static ObjectRegister &getInstance();
        static std::string generateStringID();

      public:
        bool registerObject(std::shared_ptr<const RegisteredObject> object, stringID id);
        bool unregisterObject(stringID id);

      private:
        ObjectRegister();

      private:
        std::map<stringID, std::weak_ptr<const RegisteredObject>> _register;
        static unsigned int _nbObjects;

    };

  } //namespace model
} //namespace leo
