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
  vec3 ambient;  // TODO: PBR lights models?
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

uniform sampler2D hdr;

uniform vec3 viewPos;
uniform vec3 ambientLight;
uniform sampler2D shadowMap0;
uniform mat4 lightSpaceMatrix0;

uniform float far_plane;
uniform vec3 lightPos0;
uniform samplerCube shadowCubeMap0;

const float PI = 3.14159265359;

float computeShadow(float bias, vec4 FragPosLightSpace)
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
  return 0.0;
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
  return 0.0;
  return shadow;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

void main()
{ 
  vec4 fbColor0 = texture(fb0, TexCoords);
  vec4 fbColor1 = texture(fb1, TexCoords);
  vec4 fbColor2 = texture(fb2, TexCoords);
  vec4 fbColor3 = texture(fb3, TexCoords);

  // Useful data
  vec3 position = fbColor0.xyz;
  vec3 normal = normalize(fbColor1.xyz);
  vec3 albedo = fbColor2.xyz;
  float roughness = fbColor3.x;
  float metalness = fbColor3.y;
  float ao = fbColor3.z;
  vec3 viewDir = normalize(viewPos - position);

  vec3 result = vec3(0.0, 0.0, 0.0);

  for (int i = 0; i < MAX_NUM_LIGHTS; i++)
  {
    vec3 lightPosition = vec3(5.0, 5.0, 5.0);  // Hardcoded because positions on point light dont work
    vec3 lightDir = normalize(lightPosition - position);
    vec3 halfway = normalize(lightDir + viewDir);
    float distance = length(position - lightPosition);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = attenuation * upl[i].ambient;

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metalness);
    vec3 F = fresnelSchlick(max(dot(halfway, viewDir), 0.0), F0);
    float NDF = DistributionGGX(normal, halfway, roughness);       
    float G   = GeometrySmith(normal, viewDir, lightDir, roughness);

    // Cook Torrance
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0);
    vec3 specular     = numerator / max(denominator, 0.001);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metalness;

    float NdotL = max(dot(normal, lightDir), 0.0);        
    result += (kD * albedo / PI + specular) * radiance * NdotL;

  }

  for (int i = 0; i < MAX_NUM_LIGHTS; i++)
  {
    vec3 lightDir = normalize(-udl[i].direction);
    vec3 halfway = normalize(lightDir + viewDir);
    float attenuation = 1.0;
    vec3 radiance = attenuation * udl[i].ambient;

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metalness);
    vec3 F = fresnelSchlick(max(dot(halfway, viewDir), 0.0), F0);
    float NDF = DistributionGGX(normal, halfway, roughness);       
    float G   = GeometrySmith(normal, viewDir, lightDir, roughness);

    // Cook Torrance
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0);
    vec3 specular     = numerator / max(denominator, 0.001);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metalness;

    float NdotL = max(dot(normal, lightDir), 0.0);        
    result += (kD * albedo / PI + specular) * radiance * NdotL;

  }

  FragColor = vec4(result, 1.0);
  //FragColor = vec4(texture(hdr, TexCoords).xyz, 1.0);

}
