#include "model-loader.hpp"

#include <model/entity.hpp>
#include <model/components/material.hpp>
#include <model/components/volume.hpp>
#include <model/components/drawable-collection.hpp>
#include <model/texture-manager.hpp>
#include <model/component-manager.hpp>

#include <SOIL.h>

namespace leo
{
namespace model
{

ModelLoader::ModelLoader(ComponentManager &componentManager, TextureManager &textureManager) : _componentManager(componentManager), _textureManager(textureManager)
{
}

Entity *ModelLoader::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate |
                                                     aiProcess_FlipUVs);
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode)
    {
        return nullptr;
    }
    Entity *entity = new Entity();
    textureCache.clear();
    processNode(entity, scene->mRootNode, scene);
    return entity;
}

void ModelLoader::processNode(Entity *modelNode, aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        Entity *child = new Entity();
        modelNode->addChild(child);
        child->addChild(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        Entity *newNode = new Entity();
        modelNode->addChild(newNode);
        processNode(newNode, node->mChildren[i], scene);
    }
}

Entity *ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene)
{
    Entity *entity = new Entity();
    DrawableCollection *drawables = this->_componentManager.createComponent<DrawableCollection>();
    entity->addComponent("DrawableCollection", drawables);
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // Process vertex positions, normals and texture coordinates
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }
    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    Volume *volume = this->_componentManager.createComponent<Volume>(vertices, indices);
    drawables->addDrawable(volume);

    aiMaterial *meshMaterial = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture *> diffuseMaps =
        loadMaterialTextures(meshMaterial, aiTextureType_DIFFUSE,
                             "texture_diffuse");
    std::vector<Texture *> specularMaps =
        loadMaterialTextures(meshMaterial, aiTextureType_SPECULAR,
                             "texture_specular");
    std::vector<Texture *> ambientMaps =
        loadMaterialTextures(meshMaterial, aiTextureType_AMBIENT,
                             "texture_ambient");
    Material *material = this->_componentManager.createComponent<Material>();
    if (material)
    {
        if (diffuseMaps.size())
            material->diffuse_texture = diffuseMaps[0];
        if (specularMaps.size())
            material->specular_texture = specularMaps[0];
        if (ambientMaps.size())
            material->reflection_map = ambientMaps[0];
    }

    entity->addComponent("Volume", volume);
    entity->addComponent("Material", material);
    return entity;
}

std::vector<Texture *> ModelLoader::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture *> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string path = std::string("resources/models/nanosuit/") + str.C_Str();
        bool skip = (GLboolean) false;
        for (unsigned int j = 0; j < textureCache.size(); j++)
        {
            // Check if texture is not already loaded
            Texture *t = this->_textureManager.getTexture(textureCache[j]->getId());
            if (t && t->path == path)
            {
                textures.push_back(textureCache[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        { // If texture hasn't been loaded already, load it
            Texture *t = this->_textureManager.createTexture(path.c_str());
            textures.push_back(t);
            textureCache.push_back(t); // Add to loaded _textures
        }
    }
    return textures;
}

} // namespace model
} // namespace leo