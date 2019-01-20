#pragma once

#include <model/component.hpp>
#include <map>

namespace leo {
  namespace model {

    class Globals : public Component {
      public:
        Globals();
        Globals(const Globals &other);
        const Globals &operator=(const Globals &other);

      public:
    };

  } // namespace leo
} // namespace model
