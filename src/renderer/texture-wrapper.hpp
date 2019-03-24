#pragma once

#include <renderer/global.hpp>

namespace leo
{

class Texture;

typedef struct TextureOptions
{
  GLuint textureChannels = GL_RGB;
  GLuint textureType = GL_TEXTURE_2D;
  unsigned int nbSamples = 0;
} TextureOptions;

class TextureWrapper
{
private:
  TextureWrapper();

public:
  TextureWrapper(const Texture &texture, TextureOptions textureOptions = {});
  TextureWrapper(const TextureWrapper &other);
  ~TextureWrapper();
  TextureWrapper &operator=(const TextureWrapper &other);

public:
  void init();
  bool isInitialized();
  GLuint getId() const;

private:
  bool _initialized = false;
  GLuint _id = 0;
  const Texture *_texture = nullptr;
  TextureOptions _options;
};

} // namespace leo