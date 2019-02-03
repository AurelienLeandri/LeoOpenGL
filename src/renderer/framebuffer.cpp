#include "framebuffer.hpp"

#include <utils/texture.hpp>

namespace leo
{

namespace renderer
{

Framebuffer::Framebuffer() : _id(0)
{
}

void Framebuffer::generate()
{
  this->_renderedTexture = new Texture(1620, 1080);
  this->_colorBuffers.push_back(renderer::TextureWrapper(*this->_renderedTexture));
  renderer::TextureWrapper &tw = this->_colorBuffers[this->_colorBuffers.size() - 1];
  glGenFramebuffers(1, &this->_id);
  glBindFramebuffer(GL_FRAMEBUFFER, this->_id);

  // Set "renderedTexture" as our colour attachement #0
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tw.getId(), 0);

  // The depth buffer
  GLuint depthrenderbuffer;
  glGenRenderbuffers(1, &depthrenderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1620, 1080);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tw.getId(), 0);

  // Set the list of draw buffers.
  glDrawBuffers(1, this->_drawBuffers); // "1" is the size of DrawBuffers

  // Always check that our framebuffer is ok
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "ERROR FBO" << std::endl;
    exit(1);
  }

  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::Framebuffer(const Framebuffer &other) : _id(other._id),
                                                     _colorBuffers(other._colorBuffers)
{
}

Framebuffer::~Framebuffer()
{
  glDeleteFramebuffers(1, &this->_id);
}

Framebuffer &Framebuffer::operator=(const Framebuffer &other)
{
  this->_id = other._id;
  this->_colorBuffers = other._colorBuffers;
  return *this;
}

void Framebuffer::loadFrameBuffer()
{
  glBindFramebuffer(GL_FRAMEBUFFER, this->_id);
}

} // namespace renderer

} // namespace leo
