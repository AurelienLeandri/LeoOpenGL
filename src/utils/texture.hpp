#pragma once

#include <model/registered-object.hpp>
#include <SOIL.h>

#include <string>
#include <vector>

namespace leo
{

enum TextureMode {
RGB,
SRGB,
RGBA,
SRGBA,
ERROR,
};

class Texture : public RegisteredObject
{
public:
  Texture(int witdh, int height, TextureMode mode);
  Texture(const char *path, TextureMode mode);
  Texture(const Texture &other) = delete;
  virtual ~Texture();

public:
  Texture &operator=(const Texture &other) = delete;

public:
  unsigned char *data = nullptr;
  std::string path;
  int width = 0;
  int height = 0;
  const TextureMode mode = TextureMode::ERROR;

private:
  static t_id _count;

}; // class Texture

} // namespace leo
