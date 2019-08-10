#pragma once

#include <renderer/global.hpp>

#include <utils/texture.hpp>

#include <vector>

namespace leo
{

typedef struct GLTextureOptions
{
GLuint internalFormat=GL_RGBA;
GLuint format=GL_RGBA;
GLuint type=GL_UNSIGNED_BYTE;
GLuint textureType=GL_TEXTURE_2D;
GLuint wrapping=GL_REPEAT;
} GLTextureOptions;

typedef struct TextureOptions
{
  
  unsigned int nbSamples = 4;
} TextureOptions;

class TextureWrapper
{

  using t_id = unsigned int;

private:
  TextureWrapper();

public:
  TextureWrapper(const Texture &texture, GLTextureOptions glOptions = {}, TextureOptions textureOptions = {});
  TextureWrapper(unsigned int width, unsigned int height, GLTextureOptions glOptions = {}, TextureOptions textureOptions = {});
  TextureWrapper(unsigned int width, unsigned int height, unsigned char *data, GLTextureOptions glOptions = {}, TextureOptions textureOptions = {});
  TextureWrapper(const std::vector<std::shared_ptr<Texture>> &textures, GLTextureOptions glOptions, TextureOptions textureOptions = {});
  TextureWrapper(const TextureWrapper &other);
  ~TextureWrapper();
  TextureWrapper &operator=(const TextureWrapper &other);

public:
  void init(unsigned char *data, unsigned int width, unsigned int height, const std::vector<std::shared_ptr<Texture>> *textures = nullptr);
  GLuint getId() const;
  const Texture *getTexture() const;

private:
  GLuint _id = 0;
  const Texture *_texture = nullptr;
  TextureOptions _options;
  GLTextureOptions _glOptions;
  bool _gammaCorrection = true;
};

} // namespace leo