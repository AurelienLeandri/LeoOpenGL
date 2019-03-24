#pragma once

#include <utils/texture.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <memory>

namespace leo
{

class Entity;
class Material;
class TextureManager;
class ComponentManager;
class EntityManager;

class ModelLoader
{

  using t_textureId = unsigned int;
  using t_componentId = unsigned int;

public:
  ModelLoader(EntityManager &entityManager, ComponentManager &componentManager, TextureManager &textureManager);

public:
  Entity *loadModel(std::string path, std::string objFileName);

private:
  void _processNode(Entity *modelNode, aiNode *node, const aiScene *scene, const std::string &path);
  Entity *_processMesh(aiMesh *mesh, const aiScene *scene, const std::string &path);
  std::vector<Texture *> _loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, const std::string &path);

private:
  std::vector<Texture *> textureCache;
  EntityManager &_entityManager;
  ComponentManager &_componentManager;
  TextureManager &_textureManager;
};

} // namespace leo