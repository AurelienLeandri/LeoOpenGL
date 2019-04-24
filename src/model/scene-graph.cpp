#include "scene-graph.hpp"

#include <model/entity.hpp>
#include <model/components/point-light.hpp>
#include <model/components/direction-light.hpp>

namespace leo
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

const CubeMap *SceneGraph::getCubeMap() const
{
    return this->_cubeMap;
}

void SceneGraph::setCubeMap(CubeMap *cubeMap)
{
    this->_cubeMap = cubeMap;
    this->_notify(*this, Event::CUBE_MAP_UPDATED);
}

void SceneGraph::watch(Observer *observer)
{
    Subject::watch(observer);
    if (this->_root)
        this->_root->watch(observer);
}

} // namespace leo
