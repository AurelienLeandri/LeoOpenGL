#include "model-loader.hpp"

#include <model/entity.hpp>
#include <model/components/material.hpp>
#include <model/components/volume.hpp>
#include <model/texture-manager.hpp>
#include <model/component-manager.hpp>
#include <model/entity-manager.hpp>

#include <SOIL.h>

namespace leo
{

bool pathHasExtension(const std::string &path, const char *extension);

ModelLoader::ModelLoader(EntityManager &entityManager, ComponentManager &componentManager, TextureManager &textureManager) : _entityManager(entityManager), _componentManager(componentManager), _textureManager(textureManager)
{
}

Entity *ModelLoader::loadModel(std::string path, std::string objFileName)
{
    if (path[path.length() - 1] != '/')
    {
        path = path + "/";
    }

    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path + objFileName, aiProcess_Triangulate |
                                                                   aiProcess_FlipUVs);
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode)
    {
        return nullptr;
    }
    Entity *entity = this->_entityManager.createEntity();
    textureCache.clear();
    this->_processNode(entity, scene->mRootNode, scene, path);
    return entity;
}

void ModelLoader::_processNode(Entity *modelNode, aiNode *node, const aiScene *scene, const std::string &path)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        Entity *child = this->_entityManager.createEntity();
        modelNode->addChild(child);
        child->addChild(this->_processMesh(mesh, scene, path));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        Entity *newNode = this->_entityManager.createEntity();
        modelNode->addChild(newNode);
        this->_processNode(newNode, node->mChildren[i], scene, path);
    }
}

Entity *ModelLoader::_processMesh(aiMesh *mesh, const aiScene *scene, const std::string &path)
{
    Entity *entity = this->_entityManager.createEntity();
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

    aiMaterial *meshMaterial = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture *> diffuseMaps =
        this->_loadMaterialTextures(meshMaterial, aiTextureType_DIFFUSE,
                                    "texture_diffuse", path);
    std::vector<Texture *> specularMaps =
        this->_loadMaterialTextures(meshMaterial, aiTextureType_SPECULAR,
                                    "texture_specular", path);
    std::vector<Texture *> ambientMaps =
        this->_loadMaterialTextures(meshMaterial, aiTextureType_AMBIENT,
                                    "texture_ambient", path);
    std::vector<Texture *> normalMaps =
        this->_loadMaterialTextures(meshMaterial, aiTextureType_NORMALS,
                                    "texture_normals", path);
    Material *material = this->_componentManager.createComponent<Material>();
    if (material)
    {
        if (diffuseMaps.size())
            material->diffuse_texture = diffuseMaps[0];
        if (specularMaps.size())
            material->specular_texture = specularMaps[0];
        if (ambientMaps.size())
            material->reflection_map = ambientMaps[0];
        if (normalMaps.size())
            material->normal_map = normalMaps[0];
    }

    entity->addComponent(volume);
    entity->addComponent(material);
    return entity;
}

std::vector<Texture *> ModelLoader::_loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, const std::string &path)
{
    std::vector<Texture *> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string texturePath = path + str.C_Str();
        bool skip = (GLboolean) false;
        for (unsigned int j = 0; j < textureCache.size(); j++)
        {
            // Check if texture is not already loaded
            Texture *t = this->_textureManager.getTexture(textureCache[j]->getId());
            if (t && t->path == texturePath)
            {
                textures.push_back(textureCache[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        { // If texture hasn't been loaded already, load it
            TextureMode mode;
            if (pathHasExtension(texturePath, "png"))
            {
                if (type == aiTextureType_DIFFUSE)
                    mode = TextureMode::SRGBA;
                else
                    mode = TextureMode::RGBA;
            }
            else
            {
                if (type == aiTextureType_DIFFUSE)
                    mode = TextureMode::SRGB;
                else
                    mode = TextureMode::RGB;
            }
            Texture *t = this->_textureManager.createTexture(texturePath.c_str(), mode);
            textures.push_back(t);
            textureCache.push_back(t); // Add to loaded _textures
        }
    }
    return textures;
}

bool pathHasExtension(const std::string &path, const char *extension)
{
    auto sub = path.substr(path.find_last_of(".") + 1);
    return sub == extension;
}

} // namespace leo