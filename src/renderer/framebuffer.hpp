#pragma once

#include <renderer/texture-wrapper.hpp>
#include <model/registered-object.hpp>

#include <iostream>
#include <vector>
#include <memory>

namespace leo
{

class Texture;

enum BufferType
{
  DEPTH_MAP,
  CUBE_MAP,
  TEXTURE_2D
};

typedef struct ColorBufferOptions
{
  bool hdr = false;
  unsigned int nbSamples = 1;
  unsigned int width = 1620;
  unsigned int height = 1080;
  GLuint dataFormat = GL_RGBA;
  GLuint pixelFormat = GL_RGBA;
  GLuint dataType = GL_UNSIGNED_BYTE;
  BufferType type = BufferType::TEXTURE_2D;
} ColorBufferOptions;

typedef struct DepthBufferOptions
{
  BufferType type = BufferType::DEPTH_MAP;
  unsigned int width = 1620;
  unsigned int height = 1080;
} DepthBufferOptions;

typedef struct RenderBufferOptions
{
  unsigned int nbSamples = 1;
  unsigned int width = 1620;
  unsigned int height = 1080;
} RenderBufferOptions;

class Framebuffer : public RegisteredObject
{
public:
  Framebuffer();
  Framebuffer(const Framebuffer &other);
  virtual ~Framebuffer();

public:
  Framebuffer &operator=(const Framebuffer &other);

public:
  GLuint getGLId() const { return this->_GLId; }
  const std::vector<TextureWrapper> &getColorBuffers() const { return this->_colorBuffers; }
  const TextureWrapper &getDepthBuffer() const { return *this->_depthBuffer.get(); }
  void loadFrameBuffer(GLuint bindingType = GL_FRAMEBUFFER) const;
  void addColorBuffer(ColorBufferOptions options = {});
  void setDepthBuffer(DepthBufferOptions options = {});
  void useRenderBuffer(RenderBufferOptions options = {});

private:
  GLuint _GLId = 0;
  std::vector<TextureWrapper> _colorBuffers;
  std::unique_ptr<TextureWrapper> _depthBuffer = nullptr;

private:
  static GLenum _colorAttachmentNames[4];

}; // class Framebuffer

} // namespace leo
