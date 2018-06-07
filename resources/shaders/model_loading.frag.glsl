#version 450 core

#define MAX_NUM_LIGHTS 3

struct UPointLight {
  vec3 ambient;
  float constant;
  vec3 diffuse;
  float linear;
  vec3 specular;
  float quadratic;
  vec3 position;
};

struct UDirectionLight {
  vec3 ambient;
  float constant;
  vec3 diffuse;
  float linear;
  vec3 specular;
  float quadratic;
  vec3 direction;
};

layout (std140, binding = 1) uniform s1 {
  UPointLight upl[MAX_NUM_LIGHTS];
  UDirectionLight udl[MAX_NUM_LIGHTS];
};

/*
layout (std140, binding = 1) uniform s1 {
  vec3 lposition;
  vec3 lcolor;
};
*/

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

uniform Material material;
uniform vec3 viewPos;

void main()
{
  // Light Variables
  vec3 norm = normalize(Normal);

  vec3 diffuse_sample = vec3(texture(material.texture_diffuse1, TexCoords));

  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 specular_sample = vec3(texture(material.texture_specular1, TexCoords));

  vec3 diffuse = vec3(0.0, 0.0, 0.0);
  vec3 specular = vec3(0.0, 0.0, 0.0);

  // Ambient
  vec3 ambient = material.ambient;  // TODO: fix ambient

  for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
    UPointLight iupl = upl[i];
    vec3 lightDir = normalize(iupl.position - FragPos);
    float diffuseFactor = max(dot(norm, lightDir), 0.0);
    diffuse += iupl.diffuse * diffuseFactor * (material.diffuse + diffuse_sample);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular += iupl.specular * spec * (material.specular + specular_sample);
  }
  for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
    UDirectionLight iudl = udl[i];
    vec3 lightDir = iudl.direction;
    float diffuseFactor = max(dot(norm, lightDir), 0.0);
    diffuse += iudl.diffuse * diffuseFactor * (material.diffuse + diffuse_sample);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular += iudl.specular * spec * (material.specular + specular_sample);
  }

  vec3 result = diffuse + ambient + specular;
  color = vec4(result, 1.0);
}
