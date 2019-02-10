#pragma once

namespace leo {
  namespace model {

    using t_id = unsigned int;

    class RegisteredObject {

      public:
        RegisteredObject(t_id id);

      public:
        const t_id &getId() const;

      protected:
        t_id _id;
    };


  }  // namespace leo
}  // namespace model
