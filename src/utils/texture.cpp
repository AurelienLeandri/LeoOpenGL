#include "texture.hpp"
#include "stb_image.h"

#include <iostream>

namespace leo
{

Texture::Texture(int width, int height, TextureMode textureMode)
    : RegisteredObject(), width(width), height(height), mode(textureMode)
{
}

Texture::Texture(const char *path, TextureMode mode)
    : RegisteredObject(), path(path),
      mode(mode)
{
  std::string s_path(path);
  if (s_path.substr(s_path.size() - 3, 3) == "hdr")
  {
    int nb_channels = this->mode == RGBA || this->mode == SRGBA ? 4 : 3;
    this->data = (unsigned char *) stbi_loadf(path, &this->width, &this->height, &nb_channels, 0);
  }
  else
  {
    this->data = SOIL_load_image(path,
                                 &this->width, &this->height, 0,
                                 this->mode == RGBA || this->mode == SRGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
  }
}

Texture::~Texture()
{
  if (this->data)
  {
    SOIL_free_image_data(this->data);
  }
}

} // namespace leo
