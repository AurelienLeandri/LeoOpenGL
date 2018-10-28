#version 450 core

layout (location = 0) in vec3 position;

out vec3 TexCoords;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main() {
  TexCoords = position;
  gl_Position = projection * view * vec4(position, 1.0f);
  //gl_Position = vec4(position, 1.0f);
}
