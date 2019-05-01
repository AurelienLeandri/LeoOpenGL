#include <model/texture-manager.hpp>

namespace leo
{

std::unique_ptr<Texture> TextureManager::white = std::unique_ptr<Texture>(new Texture("resources/textures/white.png", RGBA));
std::unique_ptr<Texture> TextureManager::black = std::unique_ptr<Texture>(new Texture("resources/textures/black.png", RGBA));
std::unique_ptr<Texture> TextureManager::blue = std::unique_ptr<Texture>(new Texture("resources/textures/blue.png", RGBA));

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