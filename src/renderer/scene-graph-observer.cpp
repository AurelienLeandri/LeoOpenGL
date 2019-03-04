#include <renderer/scene-graph-observer.hpp>
#include <utils/geometry.hpp>
#include <model/components/volume.hpp>

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
    if (volume) {
        notified(volume, event);
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

void SceneGraphObserver::notified(model::Entity *entity, controller::Event event)
{
}

void SceneGraphObserver::_loadComponent(model::Volume *volume)
{
    this->_renderer._loadVAO(volume);
}

} // namespace renderer
} // namespace leo
