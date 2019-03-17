#pragma once

#include <controller/observer.hpp>

#include <map>

#include <renderer/global.hpp>

#define MAX_NUM_LIGHTS 10

namespace leo
{

class Framebuffer;
class Camera;
class Shader;
class OpenGLContext;
class Texture;

class RenderNode : public Observer
{
public:
  RenderNode(OpenGLContext &context, Shader &shader, const Camera &camera);

public:
  virtual void render() = 0;
  virtual void notified(Subject *subject, Event event) = 0;

protected:
  virtual void _loadShader();
  void _loadTextureToShader(const char *uniformName, GLuint textureSlot, const Texture &texture);
  virtual void _loadOutputFramebuffer();
  virtual void _loadInputFramebuffers();
  virtual void _load() = 0;
  virtual void _unload() = 0;

public:
  std::map<std::string, Framebuffer *> &getInputs();
  const Framebuffer *getOutput() const;
  void setOutput(Framebuffer *output);

protected:
  Shader &_shader;
  const Camera &_camera;
  std::map<std::string, Framebuffer *> _inputs;
  Framebuffer *_output = nullptr;
  OpenGLContext &_context;
  GLuint _materialTextureOffset = 0;

};

} // namespace leo