#include "globals.hpp"

namespace leo {
  namespace model {

    Globals::Globals() {
    }

    Globals::Globals(const Globals &other) :
      Component(other)
    {
    }

    const Globals &Globals::operator=(const Globals &other) {
      this->_notify(controller::Event::COMPONENT_UPDATED);
      return *this;
    }

  } // namespace leo
} // namespace model
