#pragma once

#include <renderer/global.hpp>

#include <vector>

namespace leo
{

class Texture;

typedef struct TextureOptions
{
  GLuint textureType = GL_TEXTURE_2D;
  GLuint wrapping = GL_REPEAT;
  unsigned int nbSamples = 4;
} TextureOptions;

class TextureWrapper
{
private:
  TextureWrapper();

public:
  TextureWrapper(const Texture &texture, TextureOptions textureOptions = {});
  TextureWrapper(const std::vector<std::shared_ptr<Texture>> &textures, TextureOptions textureOptions = {});
  TextureWrapper(const TextureWrapper &other);
  ~TextureWrapper();
  TextureWrapper &operator=(const TextureWrapper &other);

public:
  void init(const std::vector<std::shared_ptr<Texture>> *textures = nullptr);
  GLuint getId() const;

private:
  GLuint _id = 0;
  const Texture *_texture = nullptr;
  TextureOptions _options;
  bool _gammaCorrection = true;
};

} // namespace leo