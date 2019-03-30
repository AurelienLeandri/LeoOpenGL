#include <model/texture-manager.hpp>

namespace leo
{

std::unique_ptr<Texture> TextureManager::white = std::unique_ptr<Texture>(new Texture("resources/textures/white.png"));
std::unique_ptr<Texture> TextureManager::black = std::unique_ptr<Texture>(new Texture("resources/textures/black.png"));

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    this->_textures.clear();
}

Texture *TextureManager::getTexture(t_textureId id)
{
    auto it = this->_textures.find(id);
    if (it == this->_textures.end())
    {
        return nullptr;
    }
    return it->second.get();
}

} // namespace leo