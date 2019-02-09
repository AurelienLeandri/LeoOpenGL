#pragma once

#include <model/Texture.hpp>

#include <vector>
#include <utility>

namespace leo
{
namespace model
{
class TextureManager
{
  public:
    TextureManager()
    {
    }

    ~TextureManager()
    {
        for (auto t : this->_textures)
        {
            delete t;
        }
        this->_textures.clear();
    }

    TextureManager(const TextureManager &other) = delete;
    TextureManager &operator=(const TextureManager &other) = delete;

  public:
    template <typename... ARGS>
    Texture *createTexture(ARGS &&... args)
    {
        Texture *t = new Texture(std::forward<ARGS>(args)...);
        this->_textures.push_back(t);
        return t;
    }

  private:
    std::vector<Texture *> _textures;
};
} // namespace model
} // namespace leo