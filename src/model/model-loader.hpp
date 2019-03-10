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
    Entity *loadModel(std::string path);

  private:
    void processNode(Entity *modelNode, aiNode *node, const aiScene *scene);
    Entity *processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture *> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

  private:
    std::vector<Texture *> textureCache;
    EntityManager &_entityManager;
    ComponentManager &_componentManager;
    TextureManager &_textureManager;
};

} // namespace leo