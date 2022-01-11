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

struct PBRMaterial {
  vec3 albedo_value;
  sampler2D albedo_texture;
  sampler2D normal_map;
  float metalness_value;
  sampler2D metalness_texture;
  float roughness_value;
  sampler2D roughness_texture;
  sampler2D ao_map;
  sampler2D parallax_map;
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
uniform PBRMaterial pbrMaterial;
uniform vec3 viewPos;
uniform vec3 ambientLight;
uniform samplerCube cubeMap;

void main()
{
  color = vec4(0.0, 1.0, 0.0, 1.0);
}
