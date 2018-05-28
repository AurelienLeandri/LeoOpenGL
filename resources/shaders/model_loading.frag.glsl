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
uniform Material material;
uniform vec3 viewPos;

void main()
{
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(pLight1.position - FragPos);

  // Ambient
  vec3 diffuse_sample = vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 ambient = pLight1.ambient * material.ambient * diffuse_sample;

  // Diffuse
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = pLight1.diffuse * diff * (material.diffuse + diffuse_sample);

  // Specular
  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular_sample = vec3(texture(material.texture_specular1, TexCoords));
  vec3 specular = pLight1.specular * spec * (material.specular + specular_sample);

  vec3 result = ambient + diffuse + specular;
  color = vec4(result, 1.0);
}
