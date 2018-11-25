#pragma once

#include <renderer/global.hpp>
#include <SOIL.h>

#include <string>
#include <vector>

namespace leo {

  class Texture {
    public:
      Texture();
      Texture(bool generate_empty);  // TODO, refactor texture constructors
      Texture(std::string name, std::string file_name, std::string directory);
      Texture(const Texture &other);
      virtual ~Texture();

    public:
      Texture &operator=(const Texture &other);

    public:
      static Texture createCubeMapTexture(std::string name, std::string directory);

    private:
      void _init();

    public:
      GLuint id = 0;
      std::string name;
      std::string path;  // TODO: need to store path, file name etc??
  };  // class Texture

} // namespace leo
