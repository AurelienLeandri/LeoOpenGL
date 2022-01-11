#version 450 core

#define MAX_NUM_LIGHTS 10

layout (location = 0) out vec4 Positions;
layout (location = 1) out vec4 Normals;
layout (location = 2) out vec4 Albedo;
layout (location = 3) out vec4 RoughnessMetalnessAO;

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
in vec3 FragPos;
in mat3 TBN;

uniform Material material;
uniform PBRMaterial pbrMaterial;
uniform vec3 viewPos;
uniform float far_plane;

vec2 parallaxMapping(vec2 texCoords, vec3 TSviewDir)
{
  float height_scale = 0.05;
  float height =  texture(material.parallax_map, texCoords).r;    
  vec2 p = TSviewDir.xy / TSviewDir.z * (height * height_scale);
  return texCoords - p; 
}

vec2 steepParallaxMapping(vec2 texCoords, vec3 TSviewDir)
{
  float height_scale = 0.2;
  const float minLayers = 8.0;
  const float maxLayers = 32.0;
  float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), TSviewDir)));
  float layerDepth = 1.0 / numLayers;
  float currentLayerDepth = 0.0;
  vec2 P = TSviewDir.xy * height_scale;
  vec2 deltaTexCoords = P / numLayers;
  vec2  currentTexCoords = texCoords;
  float currentDepthMapValue = texture(material.parallax_map, currentTexCoords).r;
  while(currentLayerDepth < currentDepthMapValue)
  {
    currentTexCoords -= deltaTexCoords;
    currentDepthMapValue = texture(material.parallax_map, currentTexCoords).r;  
    currentLayerDepth += layerDepth;  
  }

  vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
  float beforeDepth = texture(material.parallax_map, prevTexCoords).r - currentLayerDepth + layerDepth;
  float afterDepth  = currentDepthMapValue - currentLayerDepth;
  float weight = afterDepth / (afterDepth - beforeDepth);
  vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

  return finalTexCoords;
}


void main()
{
  // Light Variables
  vec3 tangViewDir = normalize(TBN * viewPos - TBN * FragPos);

  vec2 pTexCoords = steepParallaxMapping(TexCoords, tangViewDir);

  vec3 albedo = texture(pbrMaterial.albedo_texture, pTexCoords).xyz * pbrMaterial.albedo_value;

  float metalness = texture(pbrMaterial.metalness_texture, pTexCoords).x + pbrMaterial.metalness_value;

  float roughness = texture(pbrMaterial.roughness_texture, pTexCoords).x + pbrMaterial.roughness_value;

  float ao = texture(pbrMaterial.ao_map, pTexCoords).x;

  vec3 normal = TBN * normalize(normalize(texture(pbrMaterial.normal_map, pTexCoords).xyz) * 2.0 - 1.0);

  Positions = vec4(FragPos, 1.0);
  Normals = vec4(Normal, 1.0);
  Albedo = vec4(albedo, 1.0);
  RoughnessMetalnessAO = vec4(roughness, metalness, ao, 1.0);  // Fill when needed
}
