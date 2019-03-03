#pragma once

#include <controller/event.hpp>
#include <vector>
#include <string>

namespace leo {
  namespace controller {

    class Observer;

    class Subject {
      public:
      virtual ~Subject();

      public:
        void watch(Observer *observer);
        void unwatch(Observer *observer);
        void setObservers(std::vector<Observer *> observers);

      protected:
        void _notify(Subject &subject, Event event);
        void _notify(Event event);

      protected:
        std::vector<Observer *> _observers;
    };

  }  // namespace controller
}  // namespace leo
