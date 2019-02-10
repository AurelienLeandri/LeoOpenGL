#pragma once

#include <utils/texture.hpp>

#include <vector>
#include <utility>
#include <memory>

namespace leo
{
namespace model
{
class TextureManager
{

    using t_textureId = unsigned int;

  public:
    TextureManager()
    {
    }

    ~TextureManager()
    {
        this->_textures.clear();
    }

    TextureManager(const TextureManager &other) = delete;
    TextureManager &operator=(const TextureManager &other) = delete;

  public:
    template <typename... ARGS>
    Texture *createTexture(ARGS &&... args)
    {
        Texture *t = new Texture(std::forward<ARGS>(args)...);
        auto it = this->_textures.insert(std::pair<t_textureId, std::unique_ptr<Texture>>(t->getId(), t));
        return t;
    }

    Texture *getTexture(t_textureId id)
    {
        auto it = this->_textures.find(id);
        if (it == this->_textures.end())
        {
            return nullptr;
        }
        return it->second.get();
    }

  private:
    std::map<t_textureId, std::unique_ptr<Texture>> _textures;
};

} // namespace model
} // namespace leo