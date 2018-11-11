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
        RegisteredObject(const RegisteredObject &other) = delete;
        const RegisteredObject &operator=(const RegisteredObject &other) = delete;

      public:
        const stringID &getId() const;

      private:
        ObjectRegister &_register;
        stringID _id;
    };


  }  // namespace leo
}  // namespace model
