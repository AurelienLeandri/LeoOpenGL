#pragma once

#include <renderer/global.hpp>

#include <SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <renderer/scene/geometry-node.hpp>
#include <renderer/scene/mesh.hpp>

namespace leo {

  class Model: public GeometryNode {
    public:
      Model();
      Model(GLchar *path);
      virtual ~Model();
      Model(const Model &other);
      Model &operator=(const Model &other);

    public:
      virtual void draw(Shader *shader) override;
      virtual void update(double delta) override { UNUSED(delta); }

    private:
      void loadModel(std::string path);
      void processNode(aiNode *node, const aiScene *scene);
      Mesh processMesh(aiMesh *mesh, const aiScene *scene);
      std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                                std::string typeName);

    private:
      std::vector<Texture> _loadedTextures;
      std::vector<Mesh> _meshes;
      std::string _directory;

  };

}