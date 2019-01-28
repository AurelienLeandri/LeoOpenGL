#pragma once

#include <model/components/volume.hpp>
#include <string>
#include <vector>
#include <renderer/utils/texture.hpp>

namespace leo
{
namespace model
{

class CubeMap : public RegisteredObject
{
public:
  CubeMap(std::string name, std::string path);
  virtual ~CubeMap();
  CubeMap(const CubeMap &other) = delete;
  CubeMap &operator=(const CubeMap &other) = delete;

public:
  const std::vector<Texture *> &getTextures() const { return this->_textures; }
  const std::vector<float> &getVertices() const { return this->_vertices; }

private:
  void _loadCubeMapTextures(std::string name, std::string path);

private:
  std::vector<float> _vertices;
  std::vector<Texture *> _textures;
  GLuint _id;
};

} // namespace model
} // namespace leo
