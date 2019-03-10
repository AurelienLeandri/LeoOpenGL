#pragma once

#include <renderer/global.hpp>
#include <model/registered-object.hpp>
#include <SOIL.h>

#include <string>
#include <vector>

namespace leo
{

class Texture : public RegisteredObject
{
public:
  Texture(); // DEPRECATED
  Texture(int witdh, int height);
  Texture(bool generate_empty); // DEPRECATED
  Texture(const char *path);
  Texture(std::string name, std::string file_name, std::string directory); // TODO: DEPRECATED
  Texture(const Texture &other) = delete;
  virtual ~Texture();

public:
  Texture &operator=(const Texture &other) = delete;

public:
  static Texture createCubeMapTexture(std::string name, std::string directory);

public:
  unsigned char *data = nullptr;
  std::string path;
  int width = 0;
  int height = 0;

private:
  static t_id _count;

}; // class Texture

} // namespace leo
