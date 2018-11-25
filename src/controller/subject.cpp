#include "subject.hpp"

#include <controller/observer.hpp>

namespace leo {
  namespace controller {

    void Subject::watch(Observer *observer) {
      this->_observers.push_back(observer);
    }

    void Subject::unwatch(Observer *observer) {
      // TODO: unwatch
    }

    void Subject::notify(Event event) {
      for (auto observer : this->_observers)
        observer->notified(*this, event);
    }

  }  // namespace controller
}  // namespace leo
