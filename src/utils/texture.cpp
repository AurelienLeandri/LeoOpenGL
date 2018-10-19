#include "texture.hpp"
#include <iostream>

namespace leo {

  Texture::Texture() {
    this->_init();
    glBindTexture(GL_TEXTURE_2D, this->id);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    std::cout << "initialized text with id " << this->id << std::endl;
  }


  Texture::Texture(const Texture &other) {
    this->id = other.id;
    this->name = other.name;
    this->path = other.path;
  }

  Texture &Texture::operator=(const Texture &other) {
    this->id = other.id;
    this->name = other.name;
    this->path = other.path;
    return *this;
  }


  Texture::Texture(std::string name, std::string file_name, std::string directory) :
    name(name)
  {
    this->_init();
    std::string full = directory + '/' + file_name;
    this->path = full;

    int width, height;
    unsigned char *image = SOIL_load_image(full.c_str(),
        &width, &height, 0, SOIL_LOAD_RGBA);

    glBindTexture(GL_TEXTURE_2D, this->id);

    GLuint channels = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, channels,
        GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    SOIL_free_image_data(image);
  }


  Texture::~Texture() {
  }


  void Texture::_init() {
    glGenTextures(1, &this->id);
  }

} // namespace leo
