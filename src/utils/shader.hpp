#pragma once

#include <global.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <utils/file-reader.hpp>

namespace leo {

class Shader {
  public:
    Shader();
    Shader(const GLchar *vertexSourcePath, const GLchar *fragmentSourcePath);
    Shader(const Shader &other);
    Shader &operator=(const Shader &other);

  protected:
    void _initProgram();

  public:
    const GLuint &getProgram() const;
    void use() const;

  public:
    static void compileShader(GLuint &shader, const GLchar *shaderCode, GLint shaderType);
    static std::string generateParamName(std::string prefix, int nb, std::string suffix);

  protected:
    std::string vertexCode;
    std::string fragmentCode;
    GLuint _program;
};

}
