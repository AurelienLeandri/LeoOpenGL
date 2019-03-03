#include "subject.hpp"

#include <controller/observer.hpp>

namespace leo
{
namespace controller
{

Subject::~Subject()
{
}

void Subject::watch(Observer *observer)
{
  this->_observers.push_back(observer);
}

void Subject::unwatch(Observer *observer)
{
  // TODO: unwatch
}

void Subject::_notify(Subject &subject, Event event)
{
  for (auto observer : this->_observers)
    observer->notified(&subject, event);
}

void Subject::_notify(Event event)
{
  this->_notify(*this, event);
}

void Subject::setObservers(std::vector<Observer *> observers)
{
  this->_observers = observers;
}

} // namespace controller
} // namespace leo
