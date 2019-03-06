#pragma once

#include <controller/observer.hpp>
#include <renderer/renderer.hpp>

namespace leo
{

namespace model {
  class Component;
  class Instanced;
}

namespace renderer
{

class SceneGraphObserver : public controller::Observer
{
  public:
    SceneGraphObserver(Renderer &renderer);

  public:
    virtual void notified(controller::Subject *subject, controller::Event event);
    void notified(model::Volume *volume, controller::Event event);
    void notified(model::Light *light, controller::Event event);
    void notified(model::Entity *entity, controller::Event event);
    void notified(model::Instanced *instanced, controller::Event event);

  private:
    void _loadComponent(model::Volume *volume);
    void _loadComponent(model::Instanced *instanced);
    void _loadComponent(model::Light *light);

  private:
    Renderer &_renderer;
};

} // namespace renderer
} // namespace leo