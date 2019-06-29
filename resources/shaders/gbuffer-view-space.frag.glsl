#version 450 core

#define MAX_NUM_LIGHTS 10

layout (location = 0) out vec4 Positions;
layout (location = 1) out vec4 Normals;

struct Material {
  vec3 diffuse_value;
  sampler2D diffuse_texture;
  vec3 specular_value;
  sampler2D specular_texture;
  sampler2D reflection_map;
  sampler2D normal_map;
  sampler2D parallax_map;
  float shininess;
  vec3 emissive_value;
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

in vec2 TexCoords;
in vec3 Normal;
in vec3 NormalViewSpace;
in vec3 FragPos;
in vec4 FragPosLightSpace;
in vec3 FragPosViewSpace;
in vec3 Tangent;
in vec3 BiTangent;
in mat3 TBN;

void main()
{
  Positions = vec4(FragPosViewSpace, 1.0);
  Normals = vec4(NormalViewSpace, 1.0);
}
