#include "texture.hpp"
#include <iostream>

namespace leo
{

t_id Texture::_count = 0;

Texture::Texture(int width, int height, TextureMode textureMode)
    : RegisteredObject(_count++), width(width), height(height), mode(textureMode)
{
}

Texture::Texture(const char *path, TextureMode mode)
    : RegisteredObject(_count++), path(path),
      mode(mode == TextureMode::ERROR ? this->_getTextureModeFromPath(path) : mode)
{
  this->data = SOIL_load_image(path,
                               &this->width, &this->height, 0,
                               this->mode == RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
}

TextureMode Texture::_getTextureModeFromPath(std::string path)
{
  std::string sub = path.substr(path.find_last_of(".") + 1);
  if (sub == "jpg")
  {
    return TextureMode::RGB;
  }
  else if (sub == "png")
  {
    return TextureMode::RGBA;
  }
  else
  {
    return TextureMode::ERROR;
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
