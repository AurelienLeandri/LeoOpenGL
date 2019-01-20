#pragma once

#include <renderer/utils/texture.hpp>

namespace leo
{
namespace renderer
{

class TextureWrapper
{
  public:
    TextureWrapper(const Texture &texture);
    TextureWrapper(const TextureWrapper &other);
    ~TextureWrapper();
    TextureWrapper &operator=(const TextureWrapper &other);

  public:
    void init();
    bool isInitialized();
    GLuint getId();

    private:
    bool _initialized;
    GLuint _id;
    const Texture *_texture;
};

} // namespace renderer
} // namespace leo