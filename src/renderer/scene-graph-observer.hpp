#pragma once

#include <controller/observer.hpp>
#include <renderer/renderer.hpp>

namespace leo
{

class Component;
 class Instanced;

class SceneGraphObserver : public Observer
{
  public:
    SceneGraphObserver(Renderer &renderer);

  public:
    virtual void notified(Subject *subject, Event event);
    void notified(Volume *volume, Event event);
    void notified(Light *light, Event event);
    void notified(Entity *entity, Event event);
    void notified(Instanced *instanced, Event event);

  private:
    void _loadComponent(Volume *volume);
    void _loadComponent(Instanced *instanced);
    void _loadComponent(Light *light);

  private:
    Renderer &_renderer;
};

} // namespace leo