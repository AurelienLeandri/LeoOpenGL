#include <scene/model.hpp>

namespace leo {
  Model::Model() {
    this->_meshes.push_back(Mesh());
  }

  Model::~Model() {
  }

  void Model::draw(Material *material) {
    for (auto &m : this->_meshes) {
      m.draw(material);
    }
  }
}
