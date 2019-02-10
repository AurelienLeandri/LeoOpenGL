#include "drawable-collection.hpp"
#include <model/components/types/drawable.hpp>

namespace leo {
  namespace model {

    DrawableCollection::DrawableCollection() {
    }

    const std::map<t_id, Drawable*> &DrawableCollection::getCollection() {
      return this->_collection;
    }

    bool DrawableCollection::addDrawable(Drawable *drawable) {
      this->_notify(controller::Event::COMPONENT_UPDATED);
      this->_notify(controller::Event::COMPONENT_UPDATED);
      return this->_collection.insert(
          std::pair<t_id, Drawable*>(drawable->getId(),
            drawable)).second;
    }

    Drawable *DrawableCollection::getDrawable(t_id id) {
      auto it = this->_collection.find(id);
      if (it == this->_collection.end())
        return nullptr;
      return it->second;
    }

  } // namespace leo
} // namespace model
