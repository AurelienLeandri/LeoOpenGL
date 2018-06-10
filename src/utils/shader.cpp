#include "shader.hpp"

namespace leo {

Shader::Shader() {
}

Shader::Shader(const GLchar *vertexSourcePath,
               const GLchar *fragmentSourcePath) :
  _vertexCode(FileReader::readFile(vertexSourcePath)),
  _fragmentCode(FileReader::readFile(fragmentSourcePath))
{
  this->_initProgram();
}

Shader::Shader(const Shader &other) :
  _vertexCode(other._vertexCode),
  _fragmentCode(other._fragmentCode)
{
  this->_initProgram();
}

Shader &Shader::operator=(const Shader &other) {
  this->_vertexCode = other._vertexCode;
  this->_fragmentCode = other._fragmentCode;
  this->_initProgram();
  return *this;
}

void Shader::_initProgram() {
  const GLchar *vShaderCode = this->_vertexCode.c_str();
  const GLchar *fShaderCode = this->_fragmentCode.c_str();
  GLuint vertex, fragment;
  this->compileShader(vertex, vShaderCode, GL_VERTEX_SHADER);
  this->compileShader(fragment, fShaderCode, GL_FRAGMENT_SHADER);
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

void Shader::compileShader(GLuint &shader, const GLchar *shaderCode,
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

void Shader::use() const {
  glUseProgram(this->_program);
}

const GLuint &Shader::getProgram() const {
  return this->_program;
}

std::string Shader::generateParamName(std::string prefix, int nb, std::string suffix) {
  std::stringstream ss;
  ss << prefix << nb << suffix;
  return ss.str();
}

}
