#pragma once

#include <map>
#include <string>

namespace leo {
  namespace model {

    using stringID = std::string;

    class RegisteredObject;

    class ObjectRegister {

      public:
        static ObjectRegister &getInstance();
        static std::string generateStringID();

      public:
        bool registerObject(const RegisteredObject *object, stringID id);
        bool unregisterObject(stringID id);

      private:
        ObjectRegister();

      private:
        std::map<stringID, const RegisteredObject*> _register;
        static unsigned int _nbObjects;

    };

  } //namespace model
} //namespace leo
