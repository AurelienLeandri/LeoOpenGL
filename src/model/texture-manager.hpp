#pragma once

#include <utils/texture.hpp>

#include <vector>
#include <utility>
#include <memory>
#include <map>

namespace leo
{

class TextureManager
{

  using t_textureId = unsigned int;

public:
  TextureManager();

  ~TextureManager();

  TextureManager(const TextureManager &other) = delete;
  TextureManager &operator=(const TextureManager &other) = delete;

public:
  template <typename... ARGS>
  Texture *createTexture(ARGS &&... args);

  Texture *getTexture(t_textureId id);

public:
  static std::unique_ptr<Texture> black;
  static std::unique_ptr<Texture> white;
  static std::unique_ptr<Texture> blue;

private:
  std::map<t_textureId, std::unique_ptr<Texture>> _textures;
};

template <typename... ARGS>
Texture *TextureManager::createTexture(ARGS &&... args)
{
  Texture *t = new Texture(std::forward<ARGS>(args)...);
  auto it = this->_textures.insert(std::pair<t_textureId, std::unique_ptr<Texture>>(t->getId(), t));
  return t;
}

} // namespace leo