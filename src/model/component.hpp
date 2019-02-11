#pragma once

#include <model/icomponent.hpp>
#include <model/registered-object.hpp>
#include <controller/subject.hpp>
#include <model/type-id.hpp>

#include <map>
#include <vector>
#include <memory>

namespace leo
{
namespace model
{

template <class T>
class Component : public IComponent
{
public:
  Component() {}

public:
  virtual t_id getTypeId()
  {
    return this->typeId;
  }

public:
  static const t_id typeId;

}; // class Component

template <class T>
const t_id Component<T>::typeId = TypeId<IComponent>::get<T>();

} // namespace model
} // namespace leo
