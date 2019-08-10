#include "renderer.hpp"

#include <utils/texture.hpp>

#include <renderer/input-manager.hpp>
#include <renderer/camera.hpp>
#include <renderer/debug.hpp>
#include <renderer/main-node.hpp>
#include <renderer/cube-map-node.hpp>
#include <renderer/post-process-node.hpp>
#include <renderer/deferred-lighting-node.hpp>
#include <renderer/gaussian-blur-node.hpp>
#include <renderer/instanced-node.hpp>
#include <renderer/shadow-mapping-node.hpp>
#include <renderer/light-wrapper.hpp>

#include <model/scene-graph.hpp>
#include <model/cube-map.hpp>
#include <model/components/material.hpp>
#include <model/components/ibl.hpp>
#include <model/components/pbr-material.hpp>
#include <model/components/transformation.hpp>
#include <model/components/point-light.hpp>
#include <model/components/direction-light.hpp>
#include <model/components/volume.hpp>
#include <model/components/instanced.hpp>
#include <model/type-id.hpp>
#include <model/component-manager.hpp>

#include <sstream>
#include <algorithm>

namespace leo
{

Renderer::Renderer() : _shadowMappingShader("resources/shaders/dir-shadow-mapping.vs.glsl", "resources/shaders/dir-shadow-mapping.frag.glsl"),
                       _cubeShadowMapShader("resources/shaders/point-shadow-mapping.vs.glsl", "resources/shaders/point-shadow-mapping.frag.glsl", "resources/shaders/point-shadow-mapping.geo.glsl")
{
}

Renderer::~Renderer()
{
}

void Renderer::_visitSceneGraph()
{
  const Entity *root = this->_sceneGraph->getRoot();
  if (root)
    this->_visitSceneGraphRec(*root);
}

void Renderer::_visitSceneGraphRec(const Entity &root)
{
  for (auto &p : root.getComponents())
  {
    const IComponent *component = p.second;
    if (component)
    {
      this->_registerComponent(*component);
    }
  }
  for (auto &p : root.getChildren())
  {
    const Entity *child = p.second;
    if (child)
    {
      this->_visitSceneGraphRec(*child);
    }
  }
}

void Renderer::_registerComponent(const IComponent &component)
{
  ComponentType componentType = component.getTypeId();
  switch (componentType)
  {
  case ComponentType::DIRECTION_LIGHT:
  {
    this->_sceneContext->registerDirectionLight(*static_cast<const DirectionLight *>(&component),
                                                *this->_sceneGraph, this->_shadowMappingShader);
  }
  break;
  case ComponentType::POINT_LIGHT:
  {
    this->_sceneContext->registerPointLight(*static_cast<const PointLight *>(&component),
                                            *this->_sceneGraph, this->_cubeShadowMapShader);
  }
  break;
  case ComponentType::INSTANCED:
  {
  }
  break;
  case ComponentType::PBR_MATERIAL:
  {
    this->_sceneContext->registerMaterial(*static_cast<const PBRMaterial *>(&component));
  }
  break;
  case ComponentType::MATERIAL:
  {
    this->_sceneContext->registerMaterial(*static_cast<const Material *>(&component));
  }
  break;
  case ComponentType::TRANSFORMATION:
  {
  }
  break;
  case ComponentType::VOLUME:
  {
    this->_sceneContext->registerVolume(*static_cast<const Volume *>(&component));
  }
  break;
  case ComponentType::IBL_MAP:
  {
    this->_sceneContext->registerIBL(*static_cast<const IBL *>(&component));
  }
  break;
  default:
    break;
  }
}

void Renderer::createSceneContext(OpenGLContext &context) // TODO: Remove, create a separate class to manage observation
{
  this->_sceneContext = new SceneContext(context);
}

void Renderer::setSceneGraph(SceneGraph &sceneGraph) // TODO: Remove, create a separate class to manage observation
{
  this->_sceneGraph = &sceneGraph;
  this->_sceneGraph->watch(this);
  this->_visitSceneGraph();
}

SceneContext &Renderer::getSceneContext() // TODO: Remove, create a separate class to manage observation
{
  return *this->_sceneContext;
}

void Renderer::notified(Subject *subject, Event event)
{
  IComponent *c = dynamic_cast<IComponent *>(subject);
  if (c)
  {
    // TODO: Separate events crated, updated, deleted etc. But for now lets focus on batch mode
    this->_registerComponent(*c);
    return;
  }
  Entity *e = dynamic_cast<Entity *>(subject);
  if (e)
  {
    // TODO: Separate events crated, updated, deleted etc. But for now lets focus on batch mode
    this->_visitSceneGraphRec(*e);
    return;
  }
}

void Renderer::execute()
{
  for (auto &p : this->_sceneContext->dLights)
  {
    p.second.renderNode.render();
  }

  for (auto &p : this->_sceneContext->pLights)
  {
    p.second.renderNode.render();
  }

  std::map<t_id, int> incompleteInputs;
  std::map<t_id, bool> hasRan;
  for (auto &pair : this->_nodes)
  {
    incompleteInputs.insert(std::pair<t_id, int>(pair.first, pair.second->getInNodes().size()));
    hasRan.insert(std::pair<t_id, int>(pair.first, false));
  }
  bool change = false;
  bool finished = false;
  while (!finished)
  {
    finished = true;
    change = false;
    for (auto &p : this->_nodes)
    {
      if (!hasRan[p.first])
      {
        if (incompleteInputs[p.first] == 0)
        {
          // std::cout << p.second->getStringId() << std::endl;
          p.second->render();
          change = true;
          hasRan[p.first] = true;
          for (auto &output : p.second->getOutNodes())
          {
            incompleteInputs[output]--;
          }
        }
        else
        {
          finished = false;
        }
      }
    }
    if (!finished && !change)
    {
      std::cerr << "Incomplete RenderGraph!" << std::endl;
      exit(1);
    }
  }
}

RenderGraphNode *Renderer::getNode(t_id id)
{
  auto it = this->_nodes.find(id);
  if (it == this->_nodes.end())
  {
    return nullptr;
  }
  return it->second.get();
}

Framebuffer *Renderer::getFramebuffer(t_id id)
{
  auto it = this->_framebuffers.find(id);
  if (it == this->_framebuffers.end())
  {
    return nullptr;
  }
  return it->second.get();
}

Shader *Renderer::getShader(t_id id)
{
  auto it = this->_shaders.find(id);
  if (it == this->_shaders.end())
  {
    return nullptr;
  }
  return it->second.get();
}

} // namespace leo
