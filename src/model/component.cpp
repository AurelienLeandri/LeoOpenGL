#include "component.hpp"
#include <model/base.hpp>

namespace leo {
  namespace model {

    Component::Component()
      : RegisteredObject()
    {
      this->_notify(controller::Event::COMPONENT_CREATED);
    }

    Component::~Component() {
      this->_notify(controller::Event::COMPONENT_DELETED);
    }

    Component::Component(const Component &other) :
      RegisteredObject(other)
    {
      // TODO copy constructor
      this->_notify(controller::Event::COMPONENT_CREATED);
    }

    const Component &Component::operator=(const Component &other) {
      // TODO assignement operator
      RegisteredObject::operator=(other);
      this->_notify(controller::Event::COMPONENT_UPDATED);
      return *this;
    }

    std::shared_ptr<const Base> Component::getBase() {
      return this->_base;
    }

    void Component::setBase(const Base *base) {
      this->_notify(controller::Event::COMPONENT_UPDATED);
      this->_base = std::shared_ptr<const Base>(base);
    }

  }  // namespace leo
}  // namespace leo
