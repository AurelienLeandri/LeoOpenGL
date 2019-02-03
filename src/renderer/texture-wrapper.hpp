#pragma once

#include <utils/texture.hpp>

namespace leo
{
namespace renderer
{

class TextureWrapper
{
private:
  TextureWrapper();

public:
  TextureWrapper(const Texture &texture, bool bindData = true);
  TextureWrapper(const TextureWrapper &other);
  ~TextureWrapper();
  TextureWrapper &operator=(const TextureWrapper &other);

public:
  void init(bool bindData = true);
  bool isInitialized();
  GLuint getId() const;

private:
  bool _initialized = false;
  GLuint _id = 0;
  const Texture *_texture = nullptr;
};

} // namespace renderer
} // namespace leo