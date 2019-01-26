#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <renderer/utils/texture.hpp>

#include <string>
#include <vector>

namespace leo
{
namespace model
{

class Base;
class Material;

class ModelLoader
{
  public:
    static Base *loadModel(std::string path);

  private:
    static void processNode(Base *modelNode, aiNode *node, const aiScene *scene);
    static Base *processMesh(aiMesh *mesh, const aiScene *scene);
    static std::vector<Texture *> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

  private:
    static std::vector<Texture *> textureCache;
};

} // namespace model
} // namespace leo