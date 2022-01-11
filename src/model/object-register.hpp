#pragma once

#include <map>
#include <string>

#include "registered-object.hpp"

namespace leo
{
class ObjectRegister
{
public:
  static ObjectRegister &getInstance();
  static std::string generateStringID();

public:
  bool registerObject(const RegisteredObject *object, t_id id);
  bool unregisterObject(t_id id);

private:
  ObjectRegister();

private:
  std::map<t_id, const RegisteredObject *> _register;
  static unsigned int _nbObjects;
};

} //namespace leo
