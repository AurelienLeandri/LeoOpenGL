#version 450 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 viewPos;
uniform samplerCube cubeMap;

void main()
{
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflection = reflect(-viewDir, norm);
  color = vec4(texture(cubeMap, reflection).rgb, 1.0);
}
