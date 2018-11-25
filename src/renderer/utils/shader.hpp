#pragma once

#include <renderer/global.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <renderer/utils/file-reader.hpp>

namespace leo {

class Shader {
  public:
    Shader();
    Shader(const GLchar *vertexSourcePath, const GLchar *fragmentSourcePath);
    Shader(const GLchar *vertexSourcePath, const GLchar *fragmentSourcePath,
        const GLchar *geometrySourcePath);
    Shader(const Shader &other);
    Shader &operator=(const Shader &other);

  protected:
    void _initProgram();

  public:
    const GLuint &getProgram() const;
    void use() const;
    GLuint getTextureOffset() const { return this->_textureOffset; }
    void setTextureOffset(GLuint value) { this->_textureOffset = value; }

  public:
    static void compileShader(GLuint &shader, const GLchar *shaderCode, GLint shaderType);
    static std::string generateParamName(std::string prefix, int nb, std::string suffix);

  protected:
    std::string _vertexCode;
    std::string _fragmentCode;
    std::string _geometryCode;
    GLuint _program;
    GLuint _textureOffset = 0;
};

}
