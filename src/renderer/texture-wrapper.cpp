#include "texture-wrapper.hpp"

#include <utils/texture.hpp>

namespace leo
{

TextureWrapper::TextureWrapper(const Texture &texture, TextureOptions textureOptions)
    : _id(0), _texture(&texture), _options(textureOptions)
{
    init();
}

TextureWrapper::TextureWrapper(const std::vector<std::shared_ptr<Texture>> &textures,
                               TextureOptions textureOptions)
    : _id(0), _options(textureOptions)
{
    if (textures.size() > 0)
    {
        this->_texture = textures[0].get();
    }
    init(&textures);
}

TextureWrapper::TextureWrapper(const TextureWrapper &other)
    : _id(other._id), _texture(other._texture), _options(other._options)
{
}

TextureWrapper::~TextureWrapper()
{
}

TextureWrapper &TextureWrapper::operator=(const TextureWrapper &other)
{
    this->_id = other._id;
    this->_texture = other._texture;
    this->_options = other._options;
    return *this;
}

void TextureWrapper::init(const std::vector<std::shared_ptr<Texture>> *textures)
{
    glGenTextures(1, &this->_id);

    GLuint textureType = this->_options.textureType;
    glBindTexture(textureType, this->_id);

    GLuint channels = this->_texture->mode == TextureMode::RGB ? GL_RGB : GL_RGBA;
    unsigned char *data = this->_texture->data;
    int height = this->_texture->height;
    int width = this->_texture->width;

    if (textureType == GL_TEXTURE_CUBE_MAP)
    {
        for (int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, channels, width, height, 0, channels, GL_UNSIGNED_BYTE,
                         textures ? (*textures)[i]->data : data);
        }
    }
    else if (textureType == GL_TEXTURE_2D_MULTISAMPLE)
    {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, this->_options.nbSamples, channels, width, height, GL_TRUE);
    }
    else
    {
        glTexImage2D(textureType, 0, channels, width, height, 0, channels,
                     GL_UNSIGNED_BYTE, data ? data : 0);
    }

    glGenerateMipmap(textureType);

    GLuint wrapping = this->_options.wrapping;
    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrapping);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrapping);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_R, wrapping);

    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(textureType, 0);
}

GLuint TextureWrapper::getId() const
{
    return this->_id;
}

} // namespace leo