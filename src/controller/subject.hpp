#pragma once

#include <controller/event.hpp>
#include <vector>

namespace leo {
  namespace controller {

    class Observer;

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
