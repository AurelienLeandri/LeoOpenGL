#pragma once

#include <renderer/texture-wrapper.hpp>

#include <iostream>
#include <vector>

namespace leo
{

class Texture;

typedef struct FramebufferOptions
{
  bool multiSampled = false;
  unsigned int nbSamples = 4;
}
FramebufferOptions;

class Framebuffer
{
public:
  Framebuffer(FramebufferOptions options = {});
  Framebuffer(const Framebuffer &other);
  virtual ~Framebuffer();

public:
  Framebuffer &operator=(const Framebuffer &other);

public:
  GLuint getId() const { return this->_id; }
  const std::vector<TextureWrapper> &getColorBuffers() const { return this->_colorBuffers; }
  void loadFrameBuffer(GLuint bindingType=GL_FRAMEBUFFER) const;
  void generate();

private:
  GLuint _id = 0;
  std::vector<TextureWrapper> _colorBuffers;
  GLenum _drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  Texture *_renderedTexture = nullptr;
  const FramebufferOptions _options;

}; // class Framebuffer

} // namespace leo
