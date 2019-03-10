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

template <class T>
class Component : public IComponent
{
public:
  Component() {}

public:
  static const t_id typeId;
  const t_id getTypeId() const override { return typeId; }

}; // class Component

template <class T>
const t_id Component<T>::typeId = TypeId<IComponent>::get<Component<T>>();

} // namespace leo
