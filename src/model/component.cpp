#include "component.hpp"
#include <model/base.hpp>

namespace leo {
  namespace model {

    Component::Component()
      : RegisteredObject()
    {
    }

    Component::Component(const Component &other) :
      RegisteredObject(other)
    {
      // TODO copy constructor
    }

    const Component &Component::operator=(const Component &other) {
      // TODO assignement operator
      RegisteredObject::operator=(other);
      return *this;
    }

    std::shared_ptr<const Base> Component::getBase() {
      return this->_base;
    }

    void Component::setBase(const Base *base) {
      this->_base = std::shared_ptr<const Base>(base);
    }

  }  // namespace leo
}  // namespace leo
