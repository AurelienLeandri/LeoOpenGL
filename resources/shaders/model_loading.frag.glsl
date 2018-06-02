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

struct DirectionLight {
  float constant;
  float linear;
  float quadratic;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 direction;
};

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
  int shininess;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform PointLight pLight1;
uniform PointLight pLight2;
uniform PointLight pLight3;
uniform DirectionLight dLight1;
uniform Material material;
uniform vec3 viewPos;

void main()
{
  vec3 norm = normalize(Normal);
  vec3 lightDir1 = normalize(pLight1.position - FragPos);
  vec3 lightDir2 = normalize(pLight2.position - FragPos);
  vec3 lightDir3 = normalize(pLight3.position - FragPos);
  vec3 lightDir4 = normalize(-dLight1.direction);

  // Ambient
  vec3 diffuse_sample = vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 ambient = pLight1.ambient * (material.ambient + diffuse_sample);  // TODO: fix ambient and lights

  // Diffuse
  float diff1 = max(dot(norm, lightDir1), 0.0);
  vec3 diffuse = pLight1.diffuse * diff1 * (material.diffuse + diffuse_sample);
  float diff2 = max(dot(norm, lightDir2), 0.0);
  diffuse += pLight2.diffuse * diff2 * (material.diffuse + diffuse_sample);
  float diff3 = max(dot(norm, lightDir3), 0.0);
  diffuse += pLight3.diffuse * diff3 * (material.diffuse + diffuse_sample);
  float diff4 = max(dot(norm, lightDir4), 0.0);
  diffuse += dLight1.diffuse * diff4 * (material.diffuse + diffuse_sample);

  // Specular
  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 specular_sample = vec3(texture(material.texture_specular1, TexCoords));

  vec3 reflectDir1 = reflect(-lightDir1, norm);
  float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), material.shininess);
  vec3 specular = pLight1.specular * spec1 * (material.specular + specular_sample);
  vec3 reflectDir2 = reflect(-lightDir2, norm);
  float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), material.shininess);
  specular += pLight2.specular * spec2 * (material.specular + specular_sample);
  vec3 reflectDir3 = reflect(-lightDir3, norm);
  float spec3 = pow(max(dot(viewDir, reflectDir3), 0.0), material.shininess);
  specular += pLight3.specular * spec3 * (material.specular + specular_sample);
  vec3 reflectDir4 = reflect(-lightDir4, norm);
  float spec4 = pow(max(dot(viewDir, reflectDir4), 0.0), material.shininess);
  specular += dLight1.specular * spec4 * (material.specular + specular_sample);

  vec3 result = ambient + diffuse + specular;
  color = vec4(result, 1.0);
}
