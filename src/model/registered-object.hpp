#pragma once

#include <string>
#include <model/object-register.hpp>

namespace leo {
  namespace model {

    using stringID = std::string;

    class RegisteredObject {

      public:
        RegisteredObject(std::string prefix);
        virtual ~RegisteredObject();
        RegisteredObject(const RegisteredObject &other);
        virtual const RegisteredObject &operator=(const RegisteredObject &other) = 0;

      public:
        const stringID &getId() const;

      protected:
        ObjectRegister &_register;
        stringID _id;
    };


  }  // namespace leo
}  // namespace model
