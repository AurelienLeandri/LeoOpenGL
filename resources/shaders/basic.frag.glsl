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
  float shininess;
};

layout (std140, binding = 1) uniform s1 {
  UPointLight upl[MAX_NUM_LIGHTS];
  UDirectionLight udl[MAX_NUM_LIGHTS];
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

out vec4 color;

uniform Material material;
uniform vec3 viewPos;
uniform vec3 ambientLight;
uniform samplerCube cubeMap;
uniform sampler2D shadowMap0;

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
  vec3 ambient = ambientLight * material.diffuse_value * diffuse_sample;

  for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
    UPointLight iupl = upl[i];

    float distance = length(iupl.position - FragPos);
    // Unstable because uninitialized lights yield negative values. SHoud fix itself once we generate shader code
    float attenuation = 1.0 / (iupl.constant + iupl.linear * distance + iupl.quadratic * (distance * distance));

    vec3 lightDir = normalize(iupl.position - FragPos);
    float diffuseFactor = max(dot(norm, lightDir), 0.0);
    vec3 diffuseContribution = attenuation * (iupl.diffuse * diffuseFactor * (material.diffuse_value * diffuse_sample));
    diffuse += max(vec3(0.0), diffuseContribution);  // TODO: remove max after attenuation fix
    //vec3 reflectDir = normalize(reflect(-lightDir, norm));
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 halfwayVec = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayVec), 0.0), material.shininess);
    vec3 specularContribution = attenuation * (iupl.specular * spec * (material.specular_value * specular_sample));
    specular += max(vec3(0.0), specularContribution);  // TODO: remove max after attenuation fix
  }

  float bias = 0.005;
  for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
    UDirectionLight iudl = udl[i];
    vec3 lightDir = normalize(-iudl.direction);
    float diffuseFactor = max(dot(norm, lightDir), 0.0);
    float shadow = (1 - computeShadow(bias));
    diffuse += shadow * (iudl.diffuse * diffuseFactor * (material.diffuse_value * diffuse_sample));
    //vec3 reflectDir = normalize(reflect(-lightDir, norm));
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 halfwayVec = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayVec), 0.0), material.shininess);
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
  color = vec4(result, 1.0);
}
