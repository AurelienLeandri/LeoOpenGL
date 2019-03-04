#include <renderer/scene-graph-observer.hpp>
#include <utils/geometry.hpp>
#include <model/components/volume.hpp>
#include <model/components/point-light.hpp>
#include <model/components/direction-light.hpp>

namespace leo
{
namespace renderer
{

SceneGraphObserver::SceneGraphObserver(Renderer &renderer) : _renderer(renderer)
{
}

void SceneGraphObserver::notified(controller::Subject *c, controller::Event event)
{
    model::Volume *volume = dynamic_cast<model::Volume *>(c);
    if (volume)
    {
        notified(volume, event);
    }
    model::Light *light = dynamic_cast<model::Light *>(c);
    if (light)
    {
        notified(light, event);
    }
}

void SceneGraphObserver::notified(model::Volume *volume, controller::Event event)
{
    switch (event)
    {
    case controller::Event::COMPONENT_ADDED:
        this->_loadComponent(volume);
        break;
    default:
        break;
    }
}

void SceneGraphObserver::notified(model::Light *light, controller::Event event)
{
    switch (event)
    {
    case controller::Event::COMPONENT_ADDED:
        this->_loadComponent(light);
        break;
    default:
        break;
    }
}

void SceneGraphObserver::notified(model::Entity *entity, controller::Event event)
{
}

void SceneGraphObserver::_loadComponent(model::Volume *volume)
{
    this->_renderer._loadVAO(volume);
}

void SceneGraphObserver::_loadComponent(model::Light *light)
{
    model::DirectionLight *dl = dynamic_cast<model::DirectionLight *>(light);
    if (dl)
    {
        this->_renderer._loadLight(dl);
    }
    model::PointLight *pl = dynamic_cast<model::PointLight *>(light);
    if (pl)
    {
        this->_renderer._loadLight(pl);
    }
}

} // namespace renderer
} // namespace leo
