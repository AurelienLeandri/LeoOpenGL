#pragma once

#include <model/component.hpp>
#include <map>

namespace leo {
  namespace model {

    class Drawable;

    class DrawableCollection : public Component {
      public:
        DrawableCollection();
        const std::map<t_id, Drawable*> &getCollection();
        bool addDrawable(Drawable *drawable);
        Drawable *getDrawable(t_id id);

      private:
        std::map<t_id, Drawable*> _collection;
    };

  } // namespace leo
} // namespace model
