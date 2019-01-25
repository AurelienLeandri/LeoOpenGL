#include "cube-map.hpp"

#include <iostream>

namespace leo
{
namespace model
{

CubeMap::CubeMap(std::string name, std::string path) : _cube(Volume::createCube(2.f))
{
  this->_loadCubeMapTextures(name, path);
}

void CubeMap::_loadCubeMapTextures(std::string name, std::string path)
{
  std::vector<std::string> faces;
  faces.push_back("right.jpg");
  faces.push_back("left.jpg");
  faces.push_back("top.jpg");
  faces.push_back("bottom.jpg");
  faces.push_back("front.jpg");
  faces.push_back("back.jpg");
  for (unsigned int i = 0; i < faces.size(); i++)
  {
    std::string full = path + "/" + name + "/" + faces[i];
    this->_textures.push_back(new Texture(full.c_str()));
    if (!this->_textures[i]->data)
    {
      std::cout << "Cubemap texture failed to load at path: " << full << std::endl;
    }
  }
}

CubeMap::~CubeMap()
{
  for (auto pTexture : this->_textures) {
    delete pTexture;
  }
}

} // namespace model
} // namespace leo
