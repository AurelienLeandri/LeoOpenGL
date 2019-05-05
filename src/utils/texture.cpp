#include "texture.hpp"
#include <iostream>

namespace leo
{

t_id Texture::_count = 1;

Texture::Texture(int width, int height, TextureMode textureMode)
    : RegisteredObject(_count++), width(width), height(height), mode(textureMode)
{
}

Texture::Texture(const char *path, TextureMode mode)
    : RegisteredObject(_count++), path(path),
      mode(mode)
{
  this->data = SOIL_load_image(path,
                               &this->width, &this->height, 0,
                               this->mode == RGBA || this->mode == SRGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
}

Texture::~Texture()
{
  if (this->data)
  {
    SOIL_free_image_data(this->data);
  }
}

} // namespace leo
