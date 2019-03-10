#pragma once

#include <controller/event.hpp>

namespace leo
{

class Subject;

class Observer
{
public:
  virtual ~Observer();

public:
  virtual void notified(Subject *c, Event event) = 0;
};

} // namespace leo
