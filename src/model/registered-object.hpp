#pragma once

namespace leo
{

using t_id = unsigned int;

class RegisteredObject
{

public:
  RegisteredObject();

public:
  const t_id &getId() const;

protected:
  t_id _id;
  static t_id current_id;
};

} // namespace leo
