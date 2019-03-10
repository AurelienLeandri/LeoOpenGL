#pragma once

#include <model/entity.hpp>

#include <vector>
#include <utility>
#include <memory>

namespace leo
{

class EntityManager
{

    using t_entityId = unsigned int;

  public:
    EntityManager()
    {
    }

    ~EntityManager()
    {
        this->_entities.clear();
    }

    EntityManager(const EntityManager &other) = delete;
    EntityManager &operator=(const EntityManager &other) = delete;

  public:
    template <typename... ARGS>
    Entity *createEntity(ARGS &&... args)
    {
        Entity *e = new Entity(std::forward<ARGS>(args)...);
        auto it = this->_entities.insert(std::pair<t_entityId, std::unique_ptr<Entity>>(e->getId(), e));
        return e;
    }

    Entity *getEntity(t_entityId id) const
    {
        auto it = this->_entities.find(id);
        if (it == this->_entities.end())
        {
            return nullptr;
        }
        return it->second.get();
    }

  private:
    std::map<t_entityId, std::unique_ptr<Entity>> _entities;
};

} // namespace leo