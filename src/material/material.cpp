#include "material.hpp"

namespace leo {

Material::Material() {
}

Material::Material(const GLchar *vertexSourcePath,
               const GLchar *fragmentSourcePath) {
  std::string vertexCode = FileReader::readFile(vertexSourcePath);
  std::string fragmentCode = FileReader::readFile(fragmentSourcePath);
  const GLchar *vMaterialCode = vertexCode.c_str();
  const GLchar *fMaterialCode = fragmentCode.c_str();
  GLuint vertex, fragment;
  this->compileShader(vertex, vMaterialCode, GL_VERTEX_SHADER);
  this->compileShader(fragment, fMaterialCode, GL_FRAGMENT_SHADER);
  this->_program = glCreateProgram();
  glAttachShader(this->_program, vertex);
  glAttachShader(this->_program, fragment);
  glLinkProgram(this->_program);
  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(this->_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(this->_program, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
    << infoLog << std::endl;
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Material::compileShader(GLuint &shader, const GLchar *shaderCode,
                           GLint shaderType) {
  GLint success;
  GLchar infolog[512];
  shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderCode, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infolog);
    std::cout << "ERROR::PROGRAM::COMPILE::COMPILATION_FAILED\n"
    << infolog << std::endl;
  }
}

void Material::use() const {
  glUseProgram(this->_program);
}

const GLuint &Material::getProgram() const {
  return this->_program;
}

}
