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
        const std::map<stringID, Drawable*> &getCollection();
        bool addDrawable(Drawable *drawable);
        Drawable *getDrawable(stringID id);

      private:
        std::map<stringID, Drawable*> _collection;
    };

  } // namespace leo
} // namespace model
