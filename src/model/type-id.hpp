#pragma once

namespace leo
{
namespace model
{

using t_typeId = unsigned int;

template <typename T>
class TypeId
{
  private:
    static t_typeId _count;

  public:
    template <typename U>
    static const t_typeId get()
    {
        static const t_typeId e = _count++;
        return e;
    }

    static const t_typeId get()
    {
        return _count;
    }
};

template <typename T>
t_typeId TypeId<T>::_count = 0;

} // namespace model
} // namespace leo