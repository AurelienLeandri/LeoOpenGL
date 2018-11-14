#pragma once

#include <model/component.hpp>
#include <model/components/types/drawable.hpp>
#include <map>

namespace leo {
  namespace model {

    class DrawableCollection : public Component {
      public:
        DrawableCollection();
        DrawableCollection(const DrawableCollection &other);
        const DrawableCollection &operator=(const DrawableCollection &other);
        const std::map<stringID, std::weak_ptr<Drawable>> &getCollection();
        bool addDrawable(Drawable *drawable);
        std::shared_ptr<Drawable> getDrawable(stringID id);

      private:
        std::map<stringID, std::weak_ptr<Drawable>> _collection;
    };

  } // namespace leo
} // namespace model
