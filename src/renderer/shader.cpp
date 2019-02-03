#include "shader.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace leo
{

namespace renderer
{

Shader::Shader()
{
}

Shader::Shader(const GLchar *vertexSourcePath,
               const GLchar *fragmentSourcePath) : Shader(vertexSourcePath, fragmentSourcePath, nullptr)
{
}

Shader::Shader(const Shader &other) : _vertexCode(other._vertexCode),
                                      _fragmentCode(other._fragmentCode),
                                      _geometryCode(other._geometryCode)
{
}

Shader::Shader(const GLchar *vertexSourcePath, const GLchar *fragmentSourcePath,
               const GLchar *geometrySourcePath) : _vertexCode(FileReader::readFile(vertexSourcePath)),
                                                   _fragmentCode(FileReader::readFile(fragmentSourcePath))
{
  if (geometrySourcePath)
    _geometryCode = FileReader::readFile(geometrySourcePath);
}

Shader &Shader::operator=(const Shader &other)
{
  this->_vertexCode = other._vertexCode;
  this->_fragmentCode = other._fragmentCode;
  this->_geometryCode = other._geometryCode;
  return *this;
}

void Shader::_initProgram()
{
  const GLchar *vShaderCode = this->_vertexCode.c_str();
  const GLchar *fShaderCode = this->_fragmentCode.c_str();
  GLuint vertex, fragment;
  this->_compileShader(vertex, vShaderCode, GL_VERTEX_SHADER);
  this->_compileShader(fragment, fShaderCode, GL_FRAGMENT_SHADER);
  this->_program = glCreateProgram();
  glAttachShader(this->_program, vertex);
  glAttachShader(this->_program, fragment);
  if (this->_geometryCode.size())
  {
    GLuint geometry;
    const GLchar *gShaderCode = this->_geometryCode.c_str();
    this->_compileShader(geometry, gShaderCode, GL_GEOMETRY_SHADER);
    glAttachShader(this->_program, geometry);
  }
  glLinkProgram(this->_program);
  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(this->_program, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(this->_program, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::_compileShader(GLuint &shader, const GLchar *shaderCode,
                            GLint shaderType)
{
  GLint success;
  GLchar infolog[512];
  shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderCode, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader, 512, NULL, infolog);
    std::cout << "ERROR::PROGRAM::COMPILE::COMPILATION_FAILED\n"
              << infolog << std::endl;
  }
}

void Shader::use()
{
  if (!this->_initialized)
    this->_initProgram();
  glUseProgram(this->_program);
}

const GLuint &Shader::getProgram() const
{
  return this->_program;
}

std::string Shader::generateParamName(std::string prefix, int nb, std::string suffix)
{
  std::stringstream ss;
  ss << prefix << nb << suffix;
  return ss.str();
}

void Shader::setVector3(const char *name, glm::vec3 value)
{
  glUniform3f(glGetUniformLocation(this->_program, name), value.x, value.y, value.z);
}

void Shader::setFloat(const char *name, GLfloat value)
{
  glUniform1f(glGetUniformLocation(this->_program, name), value);
}

void Shader::setInt(const char *name, GLint value)
{
  glUniform1i(glGetUniformLocation(this->_program, name), value);
}

void Shader::setTexture(const char *name, GLuint textureId, GLuint slot, GLuint textureType)
{
  glUniform1i(glGetUniformLocation(this->_program, name), slot);
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(textureType, textureId);
}

void Shader::setMat4(const char *name, glm::mat4 value)
{
  glUniformMatrix4fv(glGetUniformLocation(this->_program, name), 1, GL_FALSE, glm::value_ptr(value));
}

} // namespace renderer

} // namespace leo