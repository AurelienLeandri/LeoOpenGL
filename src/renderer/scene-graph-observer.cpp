#include <renderer/scene-graph-observer.hpp>
#include <utils/geometry.hpp>
#include <model/components/volume.hpp>
#include <model/components/point-light.hpp>
#include <model/components/direction-light.hpp>
#include <model/components/instanced.hpp>

namespace leo
{

SceneGraphObserver::SceneGraphObserver(Renderer &renderer) : _renderer(renderer)
{
}

void SceneGraphObserver::notified(Subject *subject, Event event)
{
    IComponent *c = dynamic_cast<IComponent *>(subject);
    if (c)
    {
        switch (c->getTypeId())
        {
        case ComponentType::VOLUME:
            notified(static_cast<Volume *>(c), event);
            break;
        case ComponentType::INSTANCED:
            notified(static_cast<Instanced *>(c), event);
            break;
        case ComponentType::POINT_LIGHT:
        case ComponentType::DIRECTION_LIGHT:
            notified(dynamic_cast<Light *>(c), event);
            break;
        }
    }
}

void SceneGraphObserver::notified(Volume *volume, Event event)
{
    switch (event)
    {
    case Event::COMPONENT_ADDED:
        this->_loadComponent(volume);
        break;
    default:
        break;
    }
}

void SceneGraphObserver::notified(Light *light, Event event)
{
    switch (event)
    {
    case Event::COMPONENT_ADDED:
        this->_loadComponent(light);
        break;
    default:
        break;
    }
}

void SceneGraphObserver::notified(Instanced *instanced, Event event)
{
    switch (event)
    {
    case Event::COMPONENT_ADDED:
        this->_loadComponent(instanced);
        break;
    default:
        break;
    }
}

void SceneGraphObserver::notified(Entity *entity, Event event)
{
}

void SceneGraphObserver::_loadComponent(Volume *volume)
{
    this->_renderer._context.loadVAO(volume);
}

void SceneGraphObserver::_loadComponent(Instanced *instanced)
{
    this->_renderer._loadInstanced(instanced);
}

void SceneGraphObserver::_loadComponent(Light *light)
{
    DirectionLight *dl = dynamic_cast<DirectionLight *>(light);
    if (dl)
    {
        this->_renderer._loadLight(dl);
    }
    PointLight *pl = dynamic_cast<PointLight *>(light);
    if (pl)
    {
        this->_renderer._loadLight(pl);
    }
}

} // namespace leo
