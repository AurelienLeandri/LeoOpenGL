#version 450 core

struct PointLight {
  float constant;
  float linear;
  float quadratic;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 position;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform PointLight pLight1;
uniform vec3 viewPos;

void main()
{
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(pLight1.position - FragPos);

  // Diffuse
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * pLight1.diffuse;

  // Specular
  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);  // 32 is the shininess
  // the higher, the smaller the light dot
  vec3 specular = specularStrength * spec * pLight1.specular;

  vec3 objectColor = vec3(texture(texture_diffuse1, TexCoords));
  vec3 result = (pLight1.ambient + diffuse + specular) * objectColor;
  color = vec4(result, 1.0);
}
