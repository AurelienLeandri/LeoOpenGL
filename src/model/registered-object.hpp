#pragma once

#include <string>

namespace leo {
  namespace model {

    using stringID = std::string;

    class ObjectRegister;

    class RegisteredObject {

      public:
        RegisteredObject();

      public:
        const stringID &getId() const;

      protected:
        ObjectRegister &_register;
        stringID _id;
    };


  }  // namespace leo
}  // namespace model
