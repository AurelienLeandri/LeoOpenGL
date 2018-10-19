#pragma once

#include <global.hpp>
#include <SOIL.h>

#include <string>

namespace leo {

  class Texture {
    public:
      Texture();
      Texture(std::string name, std::string file_name, std::string directory);
      Texture(const Texture &other);
      virtual ~Texture();

    public:
      Texture &operator=(const Texture &other);

    private:
      void _init();

    public:
      GLuint id = 0;
      std::string name;
      std::string path;
  };  // class Texture

} // namespace leo
