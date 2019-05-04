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

    GLuint type = 0;
    GLuint format = 0;
    switch (this->_texture->mode)
    {
    case (RGB):
        type = format = GL_RGB;
        break;
    case (SRGB):
        type = this->_gammaCorrection ? GL_SRGB : GL_RGB;
        format = GL_RGB;
        break;
    case (RGBA):
        type = format = GL_RGBA;
        break;
    case (HDR):
        type = GL_RGBA16F;
        format = GL_RGBA;
        break;
    case (SRGBA):
        type = this->_gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
        format = GL_RGBA;
        break;
    case (DEPTH):
        type = format = GL_DEPTH_COMPONENT;
        break;
    }
    unsigned char *data = this->_texture->data;
    int height = this->_texture->height;
    int width = this->_texture->width;

    if (textureType != GL_TEXTURE_2D_MULTISAMPLE)
    { // The following is not applicable to multisampled textures
        GLuint wrapping = this->_options.wrapping;

        if (this->_texture->mode == DEPTH)
        { // Over sampling
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }
        else
        {
            glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrapping);
            glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrapping);
        }

        glTexParameteri(textureType, GL_TEXTURE_WRAP_R, wrapping);

        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, this->_texture->mode == DEPTH ? GL_NEAREST : GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, this->_texture->mode == DEPTH ? GL_NEAREST : GL_LINEAR);
    }

    if (textureType == GL_TEXTURE_CUBE_MAP)
    {
        if (this->_texture->mode == DEPTH)
        {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            for (int i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, type, width, height, 0, format, GL_FLOAT,
                             NULL);
            }
        }
        else
        {
            for (int i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, type, width, height, 0, format, GL_UNSIGNED_BYTE,
                             textures ? (*textures)[i]->data : data);
            }
        }
    }
    else if (textureType == GL_TEXTURE_2D_MULTISAMPLE)
    {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, this->_options.nbSamples, this->_texture->mode == HDR ? GL_RGBA16F : format, width, height, GL_TRUE);
    }
    else
    {
        glTexImage2D(textureType, 0, type, width, height, 0, format,
                     (this->_texture->mode == DEPTH) ? GL_FLOAT : GL_UNSIGNED_BYTE, data ? data : 0);
    }

    if (textureType != GL_TEXTURE_2D_MULTISAMPLE)
    { // The following is not applicable to multisampled textures
        glGenerateMipmap(textureType);
    }

    glBindTexture(textureType, 0);
}

GLuint TextureWrapper::getId() const
{
    return this->_id;
}

} // namespace leo