#pragma once

#include <model/components/ibl.hpp>
#include <renderer/texture-wrapper.hpp>
#include <renderer/ibl-node.hpp>

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
    IBLWrapper(const IBL &ibl, IBLNode node) : tw(ibl.getTexture(), getIBLGLOptions(), getIBLTextureOptions()), node(node)
    {
        float near = 0.01f;
        float far = 25.f;
        float aspect = 1024.f / 1024.f;
        projection = glm::perspective(glm::radians(90.0f), aspect, near, far);
        views.push_back(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
        views.push_back(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
        views.push_back(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
        views.push_back(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
        views.push_back(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
        views.push_back(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
    }

    TextureWrapper tw;
    Framebuffer map;
    std::vector<glm::mat4> views;
    glm::mat4 projection;
    IBLNode node;
} IBLWrapper;

} // namespace leo