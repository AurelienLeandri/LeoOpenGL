#pragma once

#include <controller/event.hpp>

namespace leo {
  namespace controller {

    class Subject;

    class Observer {
      public:
        virtual ~Observer();
      public:
        virtual void notified(Subject *c, Event event) = 0;
    };

  }  // namespace controller
}  // namespace leo
