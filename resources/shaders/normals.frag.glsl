#version 450 core

#define MAX_NUM_LIGHTS 10

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

void main()
{
  color = vec4(Normal, 1.0);
}
