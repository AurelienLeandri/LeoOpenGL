#include "scene-graph.hpp"
#include <model/base.hpp>

namespace leo
{
namespace model
{

SceneGraph::SceneGraph()
{
}

SceneGraph::~SceneGraph()
{
}

const Base *SceneGraph::getRoot() const
{
    return this->_root;
}

void SceneGraph::setRoot(Base *root)
{
    this->_root = root;
    this->_root->setSceneGraph(this);
}

void SceneGraph::addLight(PointLight *light)
{
    this->_pointLights.insert(std::pair<stringID, PointLight *>(light->getId(), light));
}

void SceneGraph::addLight(DirectionLight *light)
{
    this->_directionLights.insert(std::pair<stringID, DirectionLight *>(light->getId(), light));
}

void SceneGraph::removeLight(PointLight *light)
{
    this->_pointLights.erase(light->getId());
}

void SceneGraph::removeLight(DirectionLight *light)
{
    this->_directionLights.erase(light->getId());
}

const std::map<stringID, PointLight *> &SceneGraph::getPointLights() const
{
    return this->_pointLights;
}

const std::map<stringID, DirectionLight *> &SceneGraph::getDirectionLights() const
{
    return this->_directionLights;
}

} // namespace model
} // namespace leo
