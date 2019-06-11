#include "framebuffer.hpp"

#include <utils/texture.hpp>

namespace leo
{

GLenum Framebuffer::_colorAttachmentNames[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};

Framebuffer::Framebuffer() : RegisteredObject(), _GLId(0)
{
}

void Framebuffer::addColorBuffer(ColorBufferOptions options)
{
  if (!this->_GLId)
    glGenFramebuffers(1, &this->_GLId);

  glBindFramebuffer(GL_FRAMEBUFFER, this->_GLId);

  TextureOptions textureOptions;
  GLTextureOptions glOptions;

  // Special texture types
  if (options.nbSamples > 1)
  {
    glOptions.textureType = GL_TEXTURE_2D_MULTISAMPLE;
    textureOptions.nbSamples = options.nbSamples;
  }

  // Formats
  glOptions.internalFormat = options.hdr ? GL_RGBA16F : options.dataFormat;
  glOptions.format = options.pixelFormat;
  glOptions.type = options.hdr ? GL_FLOAT : options.dataType;

  this->_colorBuffers.push_back(TextureWrapper(options.width, options.height, glOptions, textureOptions));
  TextureWrapper &tw = this->_colorBuffers.back();

  // Set "renderedTexture" as our colour attachement #0
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + this->_colorBuffers.size() - 1, tw.getId(), 0);

  // Set the list of draw buffers.
  glDrawBuffers(this->_colorBuffers.size(), this->_colorAttachmentNames); // "1" is the size of DrawBuffers

  // Always check that our framebuffer is ok
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "ERROR FBO" << std::endl;
    exit(1);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::useRenderBuffer(RenderBufferOptions options)
{
  glBindFramebuffer(GL_FRAMEBUFFER, this->_GLId);

  // The depth buffer
  GLuint depthrenderbuffer;
  glGenRenderbuffers(1, &depthrenderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);

  if (options.nbSamples > 1)
  {
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, options.nbSamples, GL_DEPTH24_STENCIL8, options.width, options.height);
  }
  else
  {
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, options.width, options.height);
  }

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

  // Always check that our framebuffer is ok
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "ERROR FBO" << std::endl;
    exit(1);
  }

  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::setDepthBuffer(DepthBufferOptions options)
{
  if (!this->_GLId)
    glGenFramebuffers(1, &this->_GLId);

  glBindFramebuffer(GL_FRAMEBUFFER, this->_GLId);

  TextureOptions textureOptions;
  GLTextureOptions glOptions;

  if (options.type == DepthBufferType::CUBE_MAP) // For depth cube map
  {
    glOptions.textureType = GL_TEXTURE_CUBE_MAP;
  }

  glOptions.internalFormat = GL_DEPTH_COMPONENT;
  glOptions.format = GL_DEPTH_COMPONENT;
  glOptions.wrapping = options.type == DepthBufferType::CUBE_MAP ? GL_CLAMP_TO_EDGE : GL_CLAMP_TO_BORDER;
  glOptions.type = GL_FLOAT;

  this->_depthBuffer = std::unique_ptr<TextureWrapper>(new TextureWrapper(options.width, options.height, glOptions, textureOptions));
  TextureWrapper &tw = *this->_depthBuffer.get();

  if (options.type == DepthBufferType::DEPTH_MAP)
  {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tw.getId(), 0);
  }
  else if (options.type == DepthBufferType::CUBE_MAP)
  {
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tw.getId(), 0);
  }

  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  // Always check that our framebuffer is ok
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "ERROR FBO" << std::endl;
    exit(1);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::Framebuffer(const Framebuffer &other) : _GLId(other._GLId), _colorBuffers(other._colorBuffers)
{
  this->_depthBuffer = other._depthBuffer.get() ? std::unique_ptr<TextureWrapper>(new TextureWrapper(*other._depthBuffer.get())) : nullptr;
}

Framebuffer::~Framebuffer()
{
  glDeleteFramebuffers(1, &this->_GLId);
}

Framebuffer &Framebuffer::operator=(const Framebuffer &other)
{
  this->_GLId = other._GLId;
  this->_colorBuffers = other._colorBuffers;
  return *this;
}

void Framebuffer::loadFrameBuffer(GLuint bindingType) const
{
  glBindFramebuffer(bindingType, this->_GLId);
}

} // namespace leo
