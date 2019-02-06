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
  public:
    ComponentManager()
    {
    }

    ~ComponentManager()
    {
        for (auto c : this->_components)
        {
            delete c;
        }
        this->_components.clear();
    }

    ComponentManager(const ComponentManager &other) = delete;
    ComponentManager &operator=(const ComponentManager &other) = delete;

  public:
    template <typename T, typename... ARGS>
    T *createComponent(ARGS &&... args)
    {
        T *c = new T(std::forward<ARGS>(args)...);
        this->_components.push_back(c);
        return c;
    }

  private:
    std::vector<Component *> _components;
};
} // namespace model
} // namespace leo