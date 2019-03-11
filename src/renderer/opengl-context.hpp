#pragma once

#include <renderer/global.hpp>

#include <renderer/buffer-collection.hpp>

#include <map>

namespace leo
{

class InputManager;
class Volume;

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
    void loadVAO(const Volume *v);
    void loadVAOInstanced(const Volume *v);
    void bindVAO(const Volume *volume);

  public:
    OpenGLContext(OpenGLContext const &) = delete;
    void operator=(OpenGLContext const &) = delete;

  private:
    void _generateBufferCollection(BufferCollection &bc, const Volume &volume);

  private:
    std::map<t_id, BufferCollection> _bufferCollections;
    std::map<t_id, BufferCollection> _bufferCollectionsInstanced;
};

} // namespace leo