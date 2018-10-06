#pragma once

#include <utils/texture.hpp>
#include <iostream>
#include <vector>

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
    const std::vector<Texture> &getColorBuffers() const { return this->_colorBuffers; }

  private:
    GLuint _id;
    std::vector<Texture> _colorBuffers;

};  // class Framebuffer

}  // namespace leo
