#pragma once

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <utils/file-reader.hpp>

namespace leo {

class Material {
  public:
    Material();
    Material(const GLchar *vertexSourcePath, const GLchar *fragmentSourcePath);

  public:
    const GLuint &getProgram() const;
    void use() const;

  public:
    static void compileShader(GLuint &shader, const GLchar *shaderCode, GLint shaderType);

  protected:
    bool _hasChanged;
    std::string content;
    GLuint _program;
};

}
