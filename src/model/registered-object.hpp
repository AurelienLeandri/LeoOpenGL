#pragma once

#include <string>

namespace leo {
  namespace model {

    using t_id = unsigned int;

    class RegisteredObject {

      public:
        RegisteredObject();

      public:
        const t_id &getId() const;

      protected:
        t_id _id;
    };


  }  // namespace leo
}  // namespace model
