#pragma once

#include <model/icomponent.hpp>

#include <vector>
#include <utility>

namespace leo
{

class ComponentManager
{

  using t_componentId = unsigned int;

  public:
    ComponentManager()
    {
    }

    ~ComponentManager()
    {
        this->_components.clear();
    }

    ComponentManager(const ComponentManager &other) = delete;
    ComponentManager &operator=(const ComponentManager &other) = delete;

  public:
    template <typename T, typename... ARGS>
    T *createComponent(ARGS&&... args)
    {
        T *c = new T(std::forward<ARGS>(args)...);
        this->_components.insert(std::pair<t_componentId, std::unique_ptr<T>>(c->getId(), c));
        return c;
    }

    IComponent *getComponent(t_componentId id) {
        auto it = this->_components.find(id);
        if (it == this->_components.end()) {
            return nullptr;
        }
        return it->second.get();
    }

  private:
    std::map<t_componentId, std::unique_ptr<IComponent>> _components;
};

} // namespace leo