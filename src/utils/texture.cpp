#include "texture.hpp"
#include <iostream>

namespace leo {

  Texture::Texture() : Texture(true)
  {
  }

  Texture::Texture(int width, int height) : width(width), height(height)
  {
  }

  Texture::Texture(bool generate_empty) {
    // [DEPRECATED]
  }

  // TODO: Fix
  Texture Texture::createCubeMapTexture(std::string name, std::string directory) {
    // DEPRECATED
    Texture t(false);
    t.path = directory;
    std::vector<std::string> faces;
    faces.push_back("right.jpg");
    faces.push_back("left.jpg");
    faces.push_back("top.jpg");
    faces.push_back("bottom.jpg");
    faces.push_back("front.jpg");
    faces.push_back("back.jpg");
    //glGenTextures(1, &t.id);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, t.id);

    for (unsigned int i = 0; i < faces.size(); i++)
    {
      std::string full = directory + "/" + name + "/" + faces[i];
      t.data = SOIL_load_image(full.c_str(),
          &t.width, &t.height, 0, SOIL_LOAD_RGB);
      if (t.data) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, t.width, t.height, 0, GL_RGB, GL_UNSIGNED_BYTE, t.data
            );
      }
      else
        std::cout << "Cubemap texture failed to load at path: " << full << std::endl;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  }

  Texture::Texture(std::string name, std::string file_name, std::string directory)
  {
    // [DEPRECATED]
  }

  Texture::Texture(const char *path) : path(path)
  {
    this->data = SOIL_load_image(path,
        &this->width, &this->height, 0, SOIL_LOAD_RGBA);
  }


  Texture::~Texture() {
    if (this->data) {
      SOIL_free_image_data(this->data);
    }
  }

} // namespace leo
