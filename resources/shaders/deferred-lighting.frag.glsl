#version 450 core

#define MAX_NUM_LIGHTS 10

out vec4 FragColor;
  
in vec2 TexCoords;

// GBuffer
uniform sampler2D fb0;
uniform sampler2D fb1;
uniform sampler2D fb2;
uniform sampler2D fb3;

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

struct Material {
  vec3 diffuse_value;
  sampler2D diffuse_texture;
  vec3 specular_value;
  sampler2D specular_texture;
  sampler2D reflection_map;
  sampler2D normal_map;
  sampler2D parallax_map;
  float shininess;
};

layout (std140, binding = 1) uniform s1 {
  UPointLight upl[MAX_NUM_LIGHTS];
  UDirectionLight udl[MAX_NUM_LIGHTS];
};

in vec3 Tangent;
in vec3 BiTangent;
in vec4 FragPosLightSpace;
in mat3 TBN;

uniform Material material;
uniform vec3 viewPos;
uniform vec3 ambientLight;
uniform sampler2D shadowMap0;

uniform float far_plane;
uniform vec3 lightPos0;
uniform samplerCube shadowCubeMap0;

float computeShadow(float bias)
{
  vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;
  float closestDepth = texture(shadowMap0, projCoords.xy).r;
  float currentDepth = projCoords.z;
  vec2 texelSize = 1.0 / textureSize(shadowMap0, 0);
  float shadow = 0.0;
  for(int x = -1; x <= 1; ++x)
  {
    for(int y = -1; y <= 1; ++y)
    {
        float pcfDepth = texture(shadowMap0, projCoords.xy + vec2(x, y) * texelSize).r; 
        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
    }    
  }
  shadow /= 9.0;
  return shadow;
}

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float computePointLightShadow(float bias, vec3 FragPos)
{
  // get vector between fragment position and light position
  vec3 fragToLight = FragPos - lightPos0;
  // use the light to fragment vector to sample from the depth map    
  float currentDepth = length(fragToLight);

  vec2 texelSize = 1.0 / textureSize(shadowCubeMap0, 0);
  float offset  = 0.1;
  float shadow = 0.0;
  int samples  = 20;
  float viewDistance = length(viewPos - FragPos);
  float diskRadius = (1.0 + (viewDistance / far_plane)) / 100.0;
  for(int i = 0; i < samples; ++i)
  {
    float closestDepth = texture(shadowCubeMap0, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
    closestDepth *= far_plane;   // Undo mapping [0;1]
    if(currentDepth - bias > closestDepth)
        shadow += 1.0;
  }
  shadow /= float(samples);
  return shadow;
}

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
    vec4 fbColor0 = texture(fb0, TexCoords);
    vec4 fbColor1 = texture(fb1, TexCoords);
    vec4 fbColor2 = texture(fb2, TexCoords);
    vec4 fbColor3 = texture(fb3, TexCoords);

    vec3 FragPos = fbColor0.xyz;
    vec3 normal = fbColor1.xyz;
    vec3 ambient = ambientLight * fbColor2.xyz;
    vec3 specular_sample = fbColor3.xyz;

    vec3 color = (fbColor3).rgb;

    FragColor = vec4(color, 1.0);

}
