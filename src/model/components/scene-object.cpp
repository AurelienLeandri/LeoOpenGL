#include "scene-object.hpp"

namespace leo {
  namespace model {

    SceneObject::SceneObject() :
      _parent(), _root(std::shared_ptr<SceneObject>(this))
    {
    }

    SceneObject::~SceneObject() {
    }

    SceneObject::SceneObject(const SceneObject &other) :
      Component(other)
    {
      // TODO copy constructor
    }

    const SceneObject &SceneObject::operator=(const SceneObject &other) {
      // TODO assignement operator
      return *this;
    }

    std::map<stringID, std::shared_ptr<SceneObject>> &SceneObject::getChildren() {
      return this->_children;
    }

    bool SceneObject::addChild(SceneObject *child) {
      bool success;
      if (success = this->_children.insert(
          std::pair<stringID, std::shared_ptr<SceneObject>>(child->getId(),
            std::shared_ptr<SceneObject>(child))).second)
      {
        child->setParent(this);
        child->_setRootRec(this->_root.lock().get());
      }
      return success;
    }

    std::weak_ptr<const SceneObject> SceneObject::getParent() {
      return this->_parent;
    }

    void SceneObject::setParent(SceneObject *parent) {
      this->_parent = std::shared_ptr<SceneObject>(parent);
    }

    std::weak_ptr<const SceneObject> SceneObject::getRoot() {
      return this->_root;
    }

    void SceneObject::setRoot(SceneObject *root) {
      this->_setRootRec(root);
    }

    void SceneObject::_setRootRec(SceneObject *root) {
      this->_root = std::shared_ptr<SceneObject>(root);
      for (auto it: this->_children)
        it.second->_setRootRec(root);
    }

  }
}
