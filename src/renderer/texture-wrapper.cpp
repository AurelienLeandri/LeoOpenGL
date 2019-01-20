#include "texture-wrapper.hpp"

namespace leo
{
namespace renderer
{

TextureWrapper::TextureWrapper(const Texture &texture) : _initialized(false), _id(0), _texture(&texture)
{
    init();
}

TextureWrapper::TextureWrapper(const TextureWrapper &other) : _initialized(other._initialized), _id(other._id), _texture(other._texture) {
}

TextureWrapper::~TextureWrapper() {
}

TextureWrapper &TextureWrapper::operator=(const TextureWrapper &other) {
    this->_initialized = other._initialized;
    this->_id = other._id;
    this->_texture = other._texture;
    return *this;
}

void TextureWrapper::init() {
    glGenTextures(1, &this->_id);

    glBindTexture(GL_TEXTURE_2D, this->_id);

    GLuint channels = GL_RGBA;
    if (this->_texture->data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, channels, this->_texture->width, this->_texture->height, 0, channels,
                     GL_UNSIGNED_BYTE, this->_texture->data ? this->_texture->data : 0);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    this->_initialized = true;
}

bool TextureWrapper::isInitialized() {
    return this->_initialized;
}

GLuint TextureWrapper::getId() {
    return this->_id;
}

} // namespace renderer
} // namespace leo