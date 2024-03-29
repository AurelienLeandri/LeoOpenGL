#pragma once

#include <renderer/global.hpp>

#include <string>

namespace leo
{

class Texture;

class Shader
{
public:
  Shader();
  Shader(const GLchar *vertexSourcePath, const GLchar *fragmentSourcePath);
  Shader(const GLchar *vertexSourcePath, const GLchar *fragmentSourcePath,
         const GLchar *geometrySourcePath);
  Shader(const Shader &other);
  Shader &operator=(const Shader &other);

protected:
  void _initProgram();
  void _compileShader(GLuint &shader, const GLchar *shaderCode, GLint shaderType);

public:
  const GLuint &getProgram() const;
  void compile();
  void use();
  GLuint getTextureOffset() const { return this->_textureOffset; }
  void setTextureOffset(GLuint value) { this->_textureOffset = value; }

public:
  void setVector3(const char *name, glm::vec3 value);
  void setFloat(const char *name, GLfloat value);
  void setInt(const char *name, GLint value);
  void setTexture(const char *name, GLuint textureId, GLuint slot, GLuint textureType = GL_TEXTURE_2D);
  void setMat4(const char *name, glm::mat4 value);

public:
  static std::string generateParamName(std::string prefix, int nb, std::string suffix);

protected:
  std::string _vertexCode;
  std::string _fragmentCode;
  std::string _geometryCode;
  GLuint _program = 0;
  GLuint _textureOffset = 0;
  bool _initialized = false;
};

} // namespace leo
