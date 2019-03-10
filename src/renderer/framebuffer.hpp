#pragma once

#include <renderer/texture-wrapper.hpp>

#include <iostream>
#include <vector>

namespace leo
{

class Texture;

class Framebuffer
{
public:
  Framebuffer();
  Framebuffer(const Framebuffer &other);
  virtual ~Framebuffer();

public:
  Framebuffer &operator=(const Framebuffer &other);

public:
  void generate();
  GLuint getId() const { return this->_id; }
  const std::vector<TextureWrapper> &getColorBuffers() const { return this->_colorBuffers; }
  void loadFrameBuffer();

private:
  GLuint _id = 0;
  std::vector<TextureWrapper> _colorBuffers;
  GLenum _drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  Texture *_renderedTexture = nullptr;

}; // class Framebuffer

} // namespace leo
