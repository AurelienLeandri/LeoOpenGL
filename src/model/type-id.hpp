#pragma once

namespace leo {
namespace model {

using t_typeId = unsigned int;

template <typename T>
class TypeId {
    private:
    static t_typeId _count;

    public:
    template <typename U>
    t_typeId get() {
        static t_typeId e { _count++; }
        return e;
    }
};

}
}