#pragma once

#include <renderer/global.hpp>

#include <renderer/buffer-collection.hpp>
#include <renderer/texture-wrapper.hpp>

#include <map>

namespace leo
{

class InputManager;
class Volume;
class CubeMap;
class Texture;

typedef struct OpenGLContextOptions
{

} OpenGLContextOptions;

class OpenGLContext
{

  using t_id = unsigned int;

public:
  OpenGLContext();

public:
  void init();
  void init(const OpenGLContextOptions &options);
  void setWindowContext(GLFWwindow &window, InputManager &inputManager);
  void loadVAO(const Volume &v);
  void loadVAOInstanced(const Volume &v);
  void bindVAO(const Volume &volume);
  t_id getTextureWrapperId(const Texture &texture);
  GLuint loadCubeMap(const CubeMap &cubeMap);

public:
  OpenGLContext(OpenGLContext const &) = delete;
  void operator=(OpenGLContext const &) = delete;

private:
  void _generateBufferCollection(BufferCollection &bc, const Volume &volume);

private:
  std::map<t_id, BufferCollection> _bufferCollections;
  std::map<t_id, BufferCollection> _bufferCollectionsInstanced;
  std::map<t_id, TextureWrapper> _textures;
  BufferCollection _cubeMapBuffer;
};

} // namespace leo