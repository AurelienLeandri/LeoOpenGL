#include "framebuffer.hpp"

#include <utils/texture.hpp>

namespace leo
{

Framebuffer::Framebuffer(FramebufferOptions options) : _id(0), _options(options)
{
}

void Framebuffer::generate()
{
  if (!this->_renderedTexture && this->_options.type != FrameBufferType::CUBE_MAP)
  {
    if (this->_options.type == FrameBufferType::DEFAULT)
    {
      this->_renderedTexture = new Texture(this->_options.width, this->_options.height, this->_options.hdr ? HDR : RGBA);
    }
    else
    {
      this->_renderedTexture = new Texture(this->_options.width, this->_options.height, DEPTH);
    }
  }

  TextureOptions options;
  if (this->_options.multiSampled)
  {
    options.textureType = GL_TEXTURE_2D_MULTISAMPLE;
    options.nbSamples = this->_options.nbSamples;
  }

  if (this->_options.type == FrameBufferType::CUBE_MAP) // For depth cube map
  {
    options.textureType = GL_TEXTURE_CUBE_MAP;
    for (int i = 0; i < 6; i++)
    {
      this->_cubeMapTextures.push_back(std::make_shared<Texture>(this->_options.width, this->_options.height, DEPTH));
    }
    this->_colorBuffers.push_back(TextureWrapper(this->_cubeMapTextures, options));
    TextureWrapper &tw = this->_colorBuffers[this->_colorBuffers.size() - 1];
    glGenFramebuffers(1, &this->_id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->_id);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tw.getId(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
  }
  else
  {
    this->_colorBuffers.push_back(TextureWrapper(*this->_renderedTexture, options));
    TextureWrapper &tw = this->_colorBuffers[this->_colorBuffers.size() - 1];
    glGenFramebuffers(1, &this->_id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->_id);

    if (this->_options.type == FrameBufferType::DEFAULT)
    {
      // Set "renderedTexture" as our colour attachement #0
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, options.textureType, tw.getId(), 0);

      // The depth buffer
      GLuint depthrenderbuffer;
      glGenRenderbuffers(1, &depthrenderbuffer);
      glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);

      if (this->_options.multiSampled)
      {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, options.nbSamples, GL_DEPTH24_STENCIL8, this->_options.width, this->_options.height);
      }
      else
      {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->_options.width, this->_options.height);
      }

      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

      glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tw.getId(), 0);

      // Set the list of draw buffers.
      glDrawBuffers(1, this->_drawBuffers); // "1" is the size of DrawBuffers
    }
    else if (this->_options.type == FrameBufferType::DEPTH_MAP)
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tw.getId(), 0);
      glDrawBuffer(GL_NONE);
      glReadBuffer(GL_NONE);
    }
  }

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
                                                     _colorBuffers(other._colorBuffers),
                                                     _options(other._options)
{
}

Framebuffer::~Framebuffer()
{
  if (this->_renderedTexture)
  {
    delete this->_renderedTexture;
  }
  glDeleteFramebuffers(1, &this->_id);
}

Framebuffer &Framebuffer::operator=(const Framebuffer &other)
{
  this->_id = other._id;
  this->_colorBuffers = other._colorBuffers;
  return *this;
}

void Framebuffer::loadFrameBuffer(GLuint bindingType) const
{
  glBindFramebuffer(bindingType, this->_id);
}

const FramebufferOptions &Framebuffer::getOptions() const
{
  return this->_options;
}

} // namespace leo
