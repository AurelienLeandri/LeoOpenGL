#pragma once

#include <utils/texture.hpp>
#include <iostream>

namespace leo {

class Framebuffer {
  public:
    Framebuffer();
    Framebuffer(const Framebuffer &other) = delete;
    virtual ~Framebuffer();

  public:
    Framebuffer &operator=(const Framebuffer &other) = delete;

  public:
    GLuint getId() const { return this->_id; }
    const Texture &getRenderedTexture() const { return this->_renderedTexture; }

  private:
    GLuint _id;
    Texture _renderedTexture;

};  // class Framebuffer

}  // namespace leo
