#pragma once

#include <controller/event.hpp>
#include <vector>
#include <string>

namespace leo
{

class Observer;

class Subject
{
public:
  virtual ~Subject();

public:
  virtual void watch(Observer *observer);
  virtual void unwatch(Observer *observer);
  void setObservers(std::vector<Observer *> observers);

protected:
  void _notify(Subject &subject, Event event);
  void _notify(Event event);

protected:
  std::vector<Observer *> _observers;
};

} // namespace leo
