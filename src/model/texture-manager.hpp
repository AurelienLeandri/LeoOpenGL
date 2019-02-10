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
        std::unique_ptr<Texture> t(new Texture(std::forward<ARGS>(args)...);
        this->_textures.insert(std::pair<t_textureId, std::unique_ptr<Texture>>(t->getId(), t));
        return t;
    }

    Texture *getTexture(t_textureId id)
    {
        auto it = this->_textures.find(id);
        if (id == this->_textures.end())
        {
            return nullptr;
        }
        return it->get();
    }

  private:
    std::map<t_textureId, std::unique_ptr<Texture>> _textures;
};

t_textureId TextureManager::_count = 0;

} // namespace model
} // namespace leo