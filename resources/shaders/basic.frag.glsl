#version 450 core

#define MAX_NUM_LIGHTS 10

struct UPointLight {
  float constant;
  vec3 diffuse;
  float linear;
  vec3 specular;
  float quadratic;
  vec3 position;
};

struct UDirectionLight {
  float constant;
  vec3 diffuse;
  float linear;
  vec3 specular;
  float quadratic;
  vec3 direction;
};

struct Material {
  vec3 diffuse_value;
  sampler2D diffuse_texture;
  vec3 specular_value;
  sampler2D specular_texture;
  sampler2D reflection_map;
  int shininess;
};

layout (std140, binding = 1) uniform s1 {
  UPointLight upl[MAX_NUM_LIGHTS];
  UDirectionLight udl[MAX_NUM_LIGHTS];
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform Material material;
uniform vec3 viewPos;
uniform vec3 ambientLight;
uniform samplerCube cubeMap;

void main()
{
  // Light Variables
  vec3 norm = normalize(Normal);

  vec4 diffuse_sample_rgba = texture(material.diffuse_texture, TexCoords);
  vec3 diffuse_sample = diffuse_sample_rgba.xyz;

  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec4 specular_sample_rgba = texture(material.specular_texture, TexCoords);
  vec3 specular_sample = specular_sample_rgba.xyz;

  vec3 diffuse = vec3(0.0, 0.0, 0.0);
  vec3 specular = vec3(0.0, 0.0, 0.0);

  for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
    UPointLight iupl = upl[i];
    vec3 lightDir = normalize(iupl.position - FragPos);
    float diffuseFactor = max(dot(norm, lightDir), 0.0);
    diffuse += iupl.diffuse * diffuseFactor * (material.diffuse_value + diffuse_sample);
    vec3 reflectDir = normalize(reflect(-lightDir, norm));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular += iupl.specular * spec * (material.specular_value + specular_sample);
  }

  for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
    UDirectionLight iudl = udl[i];
    vec3 lightDir = normalize(-iudl.direction);
    //vec3 lightDir = -vec3(0.0, 0.0, 1.0);
    float diffuseFactor = max(dot(norm, lightDir), 0.0);
    diffuse += iudl.diffuse * diffuseFactor * (material.diffuse_value + diffuse_sample);
    vec3 reflectDir = normalize(reflect(-lightDir, norm));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular += iudl.specular * spec * (material.specular_value + specular_sample);
  }

  vec3 reflection = reflect(-viewDir, norm);
  vec4 reflectionColor = vec4(texture(cubeMap, reflection).rgb, 1.0);
  float reflectionFactor = texture(material.reflection_map, TexCoords).x;

  vec3 result = diffuse + specular + vec3(reflectionColor * reflectionFactor);
  color = vec4(result, 1.0);
}
