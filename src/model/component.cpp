#include "component.hpp"

#include <model/base.hpp>

namespace leo {
  namespace model {

    Component::Component()
      : RegisteredObject()
    {
      this->_notify(controller::Event::COMPONENT_CREATED);
    }

    const Base *Component::getBase() {
      return this->_base;
    }

    void Component::setBase(const Base *base) {
      this->_notify(controller::Event::COMPONENT_UPDATED);
      this->_base = base;
    }

  }  // namespace leo
}  // namespace leo
