#pragma once

#include <controller/event.hpp>
#include <vector>
#include <string>

namespace leo {
  namespace controller {

    class Observer;

    using stringID = std::string;

    class Subject {
      public:
        void watch(Observer *observer);
        void unwatch(Observer *observer);

      protected:
        void _notify(Subject &subject, Event event);
        void _notify(Event event);

      private:
        std::vector<Observer *> _observers;
    };

  }  // namespace controller
}  // namespace leo
