#include <scene/model.hpp>

namespace leo {

  Model::Model() {
    this->_meshes.push_back(Mesh());
  }

  Model::Model(GLchar *path) {
    this->loadModel(path);
  }

  Model::~Model() {
  }

  Model::Model(const Model &other) :
    GeometryNode(other),
    _loadedTextures(other._loadedTextures),
    _meshes(other._meshes),
    _directory(other._directory)
  {
  }

  Model &Model::operator=(const Model &other) {
    GeometryNode::operator=(other);
    this->_loadedTextures = other._loadedTextures;
    this->_meshes = other._meshes;
    this->_directory = other._directory;
    return *this;
  }

  void Model::draw(Shader *shader) {
    for (auto &m : this->_meshes) {
      m.draw(shader);
    }
  }

  void Model::loadModel(std::string path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate |
        aiProcess_FlipUVs);
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
      return;
    }
    this->_directory = path.substr(0, path.find_last_of('/'));
    this->processNode(scene->mRootNode, scene);
  }

  void Model::processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      this->_meshes.push_back(this->processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
      this->processNode(node->mChildren[i], scene);
  }

  Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
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
      if (mesh->mTextureCoords[0]) {
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
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }
    // Process shader
    aiMaterial *shader = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps =
      this->loadMaterialTextures(shader, aiTextureType_DIFFUSE,
          "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Texture> specularMaps =
      this->loadMaterialTextures(shader, aiTextureType_SPECULAR,
          "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    return Mesh(vertices, indices, textures);
  }

  std::vector<Texture>
    Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type,
        std::string typeName) {
      std::vector<Texture> textures;
      for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string path = str.C_Str();
        bool skip = (GLboolean) false;
        for (unsigned int j = 0; j < _loadedTextures.size(); j++) {
          // Check if texture is not already loaded
          if (_loadedTextures[j].path == path) {
            textures.push_back(_loadedTextures[j]);
            skip = true;
            break;
          }
        }
        if (!skip) {   // If texture hasn't been loaded already, load it
          Texture texture(typeName, path, this->_directory);
          textures.push_back(texture);
          this->_loadedTextures.push_back(texture);  // Add to loaded _textures
        }
      }
      return textures;
    }

}
