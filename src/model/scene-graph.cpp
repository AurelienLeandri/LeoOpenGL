#include "scene-graph.hpp"

#include <model/entity.hpp>
#include <model/components/point-light.hpp>
#include <model/components/direction-light.hpp>

namespace leo
{
namespace model
{

SceneGraph::SceneGraph()
{
}

const Entity *SceneGraph::getRoot() const
{
    return this->_root;
}

void SceneGraph::setRoot(Entity *root)
{
    this->_root = root;
    this->_root->setSceneGraph(this);
}

void SceneGraph::addLight(PointLight *light)
{
    this->_pointLights.insert(std::pair<t_id, PointLight *>(light->getId(), light));
}

void SceneGraph::addLight(DirectionLight *light)
{
    this->_directionLights.insert(std::pair<t_id, DirectionLight *>(light->getId(), light));
}

void SceneGraph::removeLight(PointLight *light)
{
    this->_pointLights.erase(light->getId());
}

void SceneGraph::removeLight(DirectionLight *light)
{
    this->_directionLights.erase(light->getId());
}

const std::map<t_id, PointLight *> &SceneGraph::getPointLights() const
{
    return this->_pointLights;
}

const std::map<t_id, DirectionLight *> &SceneGraph::getDirectionLights() const
{
    return this->_directionLights;
}

const CubeMap *SceneGraph::getCubeMap() const
{
    return this->_cubeMap;
}

void SceneGraph::setCubeMap(CubeMap *cubeMap)
{
    this->_cubeMap = cubeMap;
}

void SceneGraph::reloadScene(std::vector<controller::Observer *> observers)
{
    this->_root->reloadScene(observers);
}

} // namespace model
} // namespace leo
