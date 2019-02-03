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
    static std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

  private:
    static std::vector<std::shared_ptr<Texture>> textureCache;
};

} // namespace model
} // namespace leo