#include "drawable-collection.hpp"

namespace leo {
  namespace model {

    DrawableCollection::DrawableCollection() {
    }

    DrawableCollection::DrawableCollection(const DrawableCollection &other) :
      Component(other)
    {
    }

    const DrawableCollection &DrawableCollection::operator=(const DrawableCollection &other) {
      this->_notify(controller::Event::COMPONENT_UPDATED);
      return *this;
    }

    const std::map<stringID, Drawable*> &DrawableCollection::getCollection() {
      return this->_collection;
    }

    bool DrawableCollection::addDrawable(Drawable *drawable) {
      this->_notify(controller::Event::COMPONENT_UPDATED);
      this->_notify(controller::Event::COMPONENT_UPDATED);
      return this->_collection.insert(
          std::pair<std::string, Drawable*>(drawable->getId(),
            drawable)).second;
    }

    Drawable *DrawableCollection::getDrawable(stringID id) {
      auto it = this->_collection.find(id);
      if (it == this->_collection.end())
        return nullptr;
      return it->second;
    }

  } // namespace leo
} // namespace model
