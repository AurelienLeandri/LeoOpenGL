#pragma once

#include <model/component.hpp>

#include <vector>
#include <memory>

namespace leo {
  namespace model {

    class SceneObject : public Component {
      public:
        SceneObject();
        ~SceneObject();
        SceneObject(const SceneObject &other);
        const SceneObject &operator=(const SceneObject &other);

      public:
        std::map<stringID, std::shared_ptr<SceneObject>> &getChildren();
        bool addChild(SceneObject *child);
        std::weak_ptr<const SceneObject> getParent();
        void setParent(SceneObject *parent);
        std::weak_ptr<const SceneObject> getRoot();
        void setRoot(SceneObject *root);

      private:
        void _setRootRec(SceneObject *root);

      private:
        std::map<stringID, std::shared_ptr<SceneObject>> _children;
        std::weak_ptr<SceneObject> _parent;
        std::weak_ptr<SceneObject> _root;
    };

  }  // namespace model
}  // namespace leo
