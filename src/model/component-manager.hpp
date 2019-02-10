#pragma once

#include <model/component.hpp>

#include <vector>
#include <utility>

namespace leo
{
namespace model
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
    T *CreateComponent(ARGS &&... args)
    {
        std::unique_ptr<T> c(new T(std::forward<ARGS>(args)...);
        this->_components.insert(std::pair<t_componentId, std::unique_ptr<T>>(c->getId(), t));
        return c;
    }

    Component *getComponent(t_componentId id) {
        auto it = this->_components.find(id);
        if (id == this->_components.end()) {
            return nullptr;
        }
        return it->get();
    }

  private:
    std::map<t_componentId, std::unique_ptr<Texture>> _components;
};
} // namespace model
} // namespace leo