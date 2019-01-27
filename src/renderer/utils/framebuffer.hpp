#pragma once

#include <renderer/utils/texture.hpp>
#include <renderer/texture-wrapper.hpp>
#include <iostream>
#include <vector>

namespace leo {

class Framebuffer {
  public:
    Framebuffer();
    Framebuffer(const Framebuffer &other);
    virtual ~Framebuffer();

  public:
    Framebuffer &operator=(const Framebuffer &other);

  public:
    void generate();
    GLuint getId() const { return this->_id; }
    const std::vector<renderer::TextureWrapper> &getColorBuffers() const { return this->_colorBuffers; }
    void loadFrameBuffer();

  private:
    GLuint _id;
    std::vector<renderer::TextureWrapper> _colorBuffers;
    GLenum _drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    Texture *_renderedTexture = nullptr;

};  // class Framebuffer

}  // namespace leo
