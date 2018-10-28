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
  float ratio = 1.0 / 1.52;
  vec3 refraction = refract(-viewDir, norm, ratio);
  color = vec4(texture(cubeMap, refraction).rgb, 1.0);
}
