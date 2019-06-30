#include <model/components/ibl.hpp>
#include <utils/texture.hpp>

namespace leo
{

IBL::IBL(Texture &texture) : _texture(texture)
{
}

const Texture &IBL::getTexture() const
{
    return this->_texture;
}

} // namespace leo