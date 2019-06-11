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
DEPTH,
HDR,
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

}; // class Texture

} // namespace leo
