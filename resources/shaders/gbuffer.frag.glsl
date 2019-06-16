#version 450 core

#define MAX_NUM_LIGHTS 10

layout (location = 0) out vec4 Positions;
layout (location = 1) out vec4 Normals;
layout (location = 2) out vec4 Albedo;
layout (location = 3) out vec4 Spec;

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

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in mat3 TBN;

uniform Material material;
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

  vec4 diffuse_sample_rgba = texture(material.diffuse_texture, pTexCoords);
  vec3 diffuse_sample = diffuse_sample_rgba.xyz;

  float specularStrength = 0.5;
  vec4 specular_sample_rgba = texture(material.specular_texture, pTexCoords);
  vec3 specular_sample = specular_sample_rgba.xyz;

  vec4 normal_sample_rgba = texture(material.normal_map, pTexCoords);
  vec3 normal_sample = normalize(normal_sample_rgba.xyz);
  vec3 normal = normalize(normal_sample * 2.0 - 1.0);
  normal = TBN * normal;

  Positions = vec4(FragPos, 1.0);
  Normals = vec4(normal, 1.0);
  Albedo = vec4(material.diffuse_value * diffuse_sample, 1.0);
  Spec = vec4(material.specular_value * specular_sample, material.shininess / 100.f);  // TODO: remove ugly /100
}
