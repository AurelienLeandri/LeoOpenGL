#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out VS_OUT {
  vec2 TexCoords;
  vec3 Normal;
  vec3 FragPos;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
  vs_out.TexCoords = texCoords;
  vs_out.FragPos = vec3(model * vec4(position, 1.0));

  gl_Position = projection * view * model * vec4(position, 1.0);
  mat3 normalMatrix = mat3(transpose(inverse(view * model)));
  vs_out.Normal = normalize(vec3(projection * vec4(normalMatrix * normal, 0.0)));
}
