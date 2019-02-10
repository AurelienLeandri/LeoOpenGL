#include "cube-map.hpp"

#include <utils/texture.hpp>

#include <iostream>

namespace leo
{
namespace model
{

CubeMap::CubeMap(std::string name, std::string path)
{
  this->_vertices = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
  };
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
    this->_textures.push_back(std::make_shared<Texture>(full.c_str()));
    if (!this->_textures[i]->data)
    {
      std::cerr << "Cubemap texture failed to load at path: " << full << std::endl;
    }
  }
}

} // namespace model
} // namespace leo
