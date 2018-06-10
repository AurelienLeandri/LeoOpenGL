#include "visitor.hpp"

namespace leo {

Visitor::Visitor() {
}

Visitor::~Visitor() {
}

Visitor::Visitor(const Visitor &other) {
  UNUSED(other);
}

Visitor &Visitor::operator=(const Visitor &other) {
  UNUSED(other);
  return *this;
}

}
