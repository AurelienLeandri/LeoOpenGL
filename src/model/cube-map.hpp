#pragma once

#include <model/registered-object.hpp>

#include <renderer/global.hpp>

#include <string>
#include <vector>
#include <memory>

namespace leo
{

class Texture;

enum class CubeMapType {
  FACES,
  EQUIRECTANGULAR,
};

class CubeMap
{
public:
  CubeMap(CubeMapType type=CubeMapType::FACES);
  CubeMap(std::string name, std::string path, CubeMapType type=CubeMapType::FACES);

public:
  const std::vector<std::shared_ptr<Texture>> &getTextures() const { return this->_textures; }
  const std::vector<float> &getVertices() const { return this->_vertices; }
  CubeMapType getType() const { return this->_type; }

private:
  void _loadCubeMapTextures(std::string name, std::string path);

private:
  std::vector<float> _vertices;
  std::vector<std::shared_ptr<Texture>> _textures;
  GLuint _id = 0;
  CubeMapType _type;
};

} // namespace leo
