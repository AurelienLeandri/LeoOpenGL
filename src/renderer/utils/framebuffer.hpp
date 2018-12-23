#pragma once

#include <renderer/utils/texture.hpp>
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
    const std::vector<Texture> &getColorBuffers() const { return this->_colorBuffers; }

  private:
    GLuint _id;
    std::vector<Texture> _colorBuffers;
    GLenum _drawBuffers[1] = {GL_COLOR_ATTACHMENT0};

};  // class Framebuffer

}  // namespace leo
