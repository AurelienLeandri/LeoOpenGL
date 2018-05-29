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

  public:
    const GLuint &getProgram() const;
    void use() const;

  public:
    static void compileShader(GLuint &shader, const GLchar *shaderCode, GLint shaderType);
    static std::string generateParamName(std::string prefix, int nb, std::string suffix);

  protected:
    bool _hasChanged;
    std::string content;
    GLuint _program;
};

}
