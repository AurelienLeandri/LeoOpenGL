#include "texture-wrapper.hpp"

namespace leo
{

TextureWrapper::TextureWrapper(const Texture &texture, GLTextureOptions glOptions, TextureOptions textureOptions)
    : _id(0), _texture(&texture), _glOptions(glOptions), _options(textureOptions)
{
    init(texture.data, texture.width, texture.height);
}

TextureWrapper::TextureWrapper(unsigned int width, unsigned int height, GLTextureOptions glOptions, TextureOptions textureOptions)
    : _id(0), _glOptions(glOptions), _options(textureOptions)
{
    init(nullptr, width, height);
}

TextureWrapper::TextureWrapper(const std::vector<std::shared_ptr<Texture>> &textures, GLTextureOptions glOptions,
                               TextureOptions textureOptions)
    : _id(0), _glOptions(glOptions), _options(textureOptions)
{
    if (textures.size() > 0)
    {
        this->_texture = textures[0].get();
    }
    init(textures[0]->data, textures[0]->width, textures[0]->height, &textures);
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
    this->_glOptions = other._glOptions;
    this->_options = other._options;
    return *this;
}

void TextureWrapper::init(unsigned char *data, unsigned int width, unsigned int height, const std::vector<std::shared_ptr<Texture>> *textures)
{
    glGenTextures(1, &this->_id);

    GLuint textureType = this->_glOptions.textureType;
    glBindTexture(textureType, this->_id);
    GLuint internalFormat = this->_glOptions.internalFormat;
    GLuint format = this->_glOptions.format;
    GLuint type = this->_glOptions.type;

    if (textureType != GL_TEXTURE_2D_MULTISAMPLE)
    { // The following is not applicable to multisampled textures
        GLuint wrapping = this->_glOptions.wrapping;
        glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrapping);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrapping);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_R, wrapping);
        if (internalFormat == GL_DEPTH_COMPONENT)
        {
            float borderColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, internalFormat == GL_DEPTH_COMPONENT ? GL_NEAREST : GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, internalFormat == GL_DEPTH_COMPONENT ? GL_NEAREST : GL_LINEAR);
    }

    if (textureType == GL_TEXTURE_CUBE_MAP)
    {
        for (int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, internalFormat, width, height, 0, format, type,
                         textures ? (*textures)[i]->data : data);
        }
    }
    else if (textureType == GL_TEXTURE_2D_MULTISAMPLE)
    {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, this->_options.nbSamples, format, width, height, GL_TRUE);
    }
    else  // GL_TEXTURE_2D
    {
        glTexImage2D(textureType, 0, internalFormat, width, height, 0, format,
                     type, data ? data : 0);
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