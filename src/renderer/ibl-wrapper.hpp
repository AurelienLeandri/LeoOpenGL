#pragma once

#include <model/components/ibl.hpp>
#include <renderer/texture-wrapper.hpp>

namespace leo
{

namespace
{
TextureOptions getIBLTextureOptions()
{
    TextureOptions textureOptions;
    textureOptions.nbSamples = 1;
    return textureOptions;
}

GLTextureOptions getIBLGLOptions()
{
    GLTextureOptions glOptions;
    glOptions.format = GL_RGB;
    glOptions.internalFormat = GL_RGB16F;
    glOptions.type = GL_FLOAT;
    return glOptions;
}
} // namespace

typedef struct IBLWrapper
{
    IBLWrapper(const IBL &ibl) : tw(ibl.getTexture(), getIBLGLOptions(), getIBLTextureOptions())
    {
    }

    TextureWrapper tw;
} IBLWrapper;

} // namespace leo