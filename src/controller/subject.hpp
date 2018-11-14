#pragma once

#include <controller/event.hpp>
#include <controller/observer.hpp>

namespace leo {
  namespace controller {

    class Subject {
      public:
        void watch(Observer *observer);
        void unwatch(Observer *observer);
        void notify(Event event);

      private:
        std::vector<Observer *> _observers;
    };

  }  // namespace controller
}  // namespace leo
