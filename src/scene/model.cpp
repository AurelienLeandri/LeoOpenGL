#include <scene/model.hpp>

namespace leo {

  GLint TextureFromFile(const char *path, std::string directory);

  Model::Model() {
    this->_meshes.push_back(Mesh());
  }

  Model::Model(GLchar *path) {
    this->loadModel(path);
  }

  Model::~Model() {
  }

  Model::Model(const Model &other) :
    textures_loaded(other.textures_loaded),
    _meshes(other._meshes),
    directory(other.directory)
  {
  }

  Model &Model::operator=(const Model &other) {
    this->textures_loaded = other.textures_loaded;
    this->_meshes = other._meshes;
    this->directory = other.directory;
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
    this->directory = path.substr(0, path.find_last_of('/'));
    this->processNode(scene->mRootNode, scene);
  }

  void Model::processNode(aiNode *node, const aiScene *scene) {
    for (GLuint i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      this->_meshes.push_back(this->processMesh(mesh, scene));
    }
    for (GLuint i = 0; i < node->mNumChildren; i++)
      this->processNode(node->mChildren[i], scene);
  }

  Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (GLuint i = 0; i < mesh->mNumVertices; i++) {
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
    for (GLuint i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (GLuint j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }
    // Process shader
    if (mesh->mMaterialIndex >= 0) {
      aiMaterial *shader = scene->mMaterials[mesh->mMaterialIndex];
      std::vector<Texture> diffuseMaps =
        this->loadMaterialTextures(shader, aiTextureType_DIFFUSE,
            "texture_diffuse");
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
      std::vector<Texture> specularMaps =
        this->loadMaterialTextures(shader, aiTextureType_SPECULAR,
            "texture_specular");
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    return Mesh(vertices, indices, textures);
  }

  std::vector<Texture>
    Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type,
        std::string typeName) {
      std::vector<Texture> textures;
      for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        GLboolean skip = (GLboolean) false;
        for (GLuint j = 0; j < textures_loaded.size(); j++) {
          // Check if texture is not already loaded
          if (textures_loaded[j].path == str) {
            textures.push_back(textures_loaded[j]);
            skip = (GLboolean) true;
            break;
          }
        }
        if (!skip) {   // If texture hasn't been loaded already, load it
          Texture texture;
          texture.id = (GLuint) TextureFromFile(str.C_Str(), this->directory);
          texture.type = typeName;
          texture.path = str;
          textures.push_back(texture);
          this->textures_loaded.push_back(texture);  // Add to loaded _textures
        }
      }
      return textures;
    }

  GLint TextureFromFile(const char *path, std::string directory) {
    //Generate texture ID and load texture data
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height;
    std::cout << filename.c_str() << std::endl;
    unsigned char *image = SOIL_load_image(filename.c_str(), &width, &height,
        0, SOIL_LOAD_RGB);  // TODO: RGBA for png ??
    std::cout << (SOIL_last_result()) << std::endl;
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
        GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
  }
}