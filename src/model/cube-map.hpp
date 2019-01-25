#pragma once

#include <model/components/volume.hpp>
#include <string>
#include <vector>
#include <renderer/utils/texture.hpp>

namespace leo
{
namespace model
{

class CubeMap
{
public:
  CubeMap(std::string name, std::string path);
  virtual ~CubeMap();
  CubeMap(const CubeMap &other) = delete;
  CubeMap &operator=(const CubeMap &other) = delete;

public:
  const std::vector<Texture *> &getTextures() const { return this->_textures; }
  const Volume *getCube() const { return &this->_cube; }

private:
  void _loadCubeMapTextures(std::string name, std::string path);

private:
  Volume _cube;
  std::vector<Texture *> _textures;
};

} // namespace model
} // namespace leo
