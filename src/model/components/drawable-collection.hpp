#pragma once

#include <model/component.hpp>
#include <map>

namespace leo {
  namespace model {

    class Drawable;

    class DrawableCollection : public Component {
      public:
        DrawableCollection();
        const std::map<stringID, Drawable*> &getCollection();
        bool addDrawable(Drawable *drawable);
        Drawable *getDrawable(stringID id);

      private:
        std::map<stringID, Drawable*> _collection;
    };

  } // namespace leo
} // namespace model
