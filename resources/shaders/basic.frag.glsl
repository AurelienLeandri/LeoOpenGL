#version 450 core

#define MAX_NUM_LIGHTS 10

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

in vec2 TexCoords;
in vec3 Normal;
in vec3 Tangent;
in vec3 BiTangent;
in vec3 FragPos;
in vec4 FragPosLightSpace;
in mat3 TBN;

out vec4 color;

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

float computePointLightShadow(float bias)
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
  return texCoords;
  //return (TSviewDir * (texture(material.parallax_map, texCoords).r / length(TSviewDir))).xy + texCoords;
}


void main()
{
  // Light Variables
  vec3 norm = normalize(Normal);
  vec3 tang = normalize(Tangent);
  vec3 bita = normalize(BiTangent);

  vec3 viewDir = normalize(viewPos - FragPos);

  vec2 pTexCoords = parallaxMapping(TexCoords, TBN * viewDir);

  vec4 diffuse_sample_rgba = texture(material.diffuse_texture, pTexCoords);
  vec3 diffuse_sample = diffuse_sample_rgba.xyz;

  float specularStrength = 0.5;
  vec4 specular_sample_rgba = texture(material.specular_texture, pTexCoords);
  vec3 specular_sample = specular_sample_rgba.xyz;

  vec4 normal_sample_rgba = texture(material.normal_map, pTexCoords);
  vec3 normal_sample = normalize(normal_sample_rgba.xyz);
  vec3 normal = normalize(normal_sample * 2.0 - 1.0);
  normal = TBN * normal;

  vec3 diffuse = vec3(0.0, 0.0, 0.0);
  vec3 specular = vec3(0.0, 0.0, 0.0);
  vec3 ambient = ambientLight * material.diffuse_value * diffuse_sample;

  float bias = 0.01;

  for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
    UPointLight iupl = upl[i];

    float distance = length(iupl.position - FragPos);
    // Unstable because uninitialized lights yield negative values. SHoud fix itself once we generate shader code
    float attenuation = 1.0 / (iupl.constant + iupl.linear * distance + iupl.quadratic * (distance * distance));

    vec3 lightDir = normalize(iupl.position - FragPos);
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuseContribution = attenuation * (iupl.diffuse * diffuseFactor * (material.diffuse_value * diffuse_sample));
    float shadow = (1 - computePointLightShadow(bias));
    diffuse += shadow * (max(vec3(0.0), diffuseContribution));  // TODO: remove max after attenuation fix
    //vec3 reflectDir = normalize(reflect(-lightDir, norm));
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 halfwayVec = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayVec), 0.0), material.shininess);
    vec3 specularContribution = attenuation * (iupl.specular * spec * (material.specular_value * specular_sample));
    specular += shadow * (max(vec3(0.0), specularContribution));  // TODO: remove max after attenuation fix
  }

  for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
    UDirectionLight iudl = udl[i];
    vec3 lightDir = normalize(-iudl.direction);
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    float shadow = (1 - computeShadow(bias));
    diffuse += shadow * (iudl.diffuse * diffuseFactor * (material.diffuse_value * diffuse_sample));
    //vec3 reflectDir = normalize(reflect(-lightDir, norm));
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 halfwayVec = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayVec), 0.0), material.shininess);
    specular += shadow * (iudl.specular * spec * (material.specular_value * specular_sample));
  }

  /*
  vec3 reflection = reflect(-viewDir, norm);
  vec4 reflectionColor = vec4(texture(cubeMap, reflection).rgb, 1.0);
  float reflectionFactor = texture(material.reflection_map, TexCoords).x;
  */

  vec3 result = diffuse + specular + (ambient * diffuse_sample)/* + vec3(reflectionColor * reflectionFactor)*/;
  //vec3 result = vec3(material.shininess / 100.0);
  //color = vec4(FragPosLightSpace, 1.0);
  //color = vec4(diffuse_value, 1.0);
  //color = vec4(diffuse_sample, 1.0);
  //color = vec4(udl[0].diffuse, 1.0);

  //color = vec4(0.0, 1.0, 0.0, 1.0);
  //color = vec4(texture(material.diffuse_texture, TexCoords).rgb, 1.0);
  //color = vec4(texture(material.specular_texture, TexCoords).rgb, 1.0);
  //color = vec4(texture(material.reflection_map, TexCoords).rgb, 1.0);

  //color = vec4(vec3(closestDepth), 1.0);
  color = vec4(result, 1.0);
}
