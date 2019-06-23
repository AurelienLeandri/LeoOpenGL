#version 450 core

#define MAX_NUM_LIGHTS 10

out vec4 FragColor;
  
in vec2 TexCoords;

// GBuffer
uniform sampler2D fb0;
uniform sampler2D fb1;
uniform sampler2D fb2;
uniform sampler2D fb3;
uniform sampler2D occlusionMap;

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

uniform vec3 viewPos;
uniform vec3 ambientLight;
uniform sampler2D shadowMap0;
uniform mat4 lightSpaceMatrix0;

uniform float far_plane;
uniform vec3 lightPos0;
uniform samplerCube shadowCubeMap0;

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

void main()
{ 
  vec4 fbColor0 = texture(fb0, TexCoords);
  vec4 fbColor1 = texture(fb1, TexCoords);
  vec4 fbColor2 = texture(fb2, TexCoords);
  vec4 fbColor3 = texture(fb3, TexCoords);

  //float occlusion = texture(occlusionMap, TexCoords).x;
  float occlusion = 1.0;


  vec3 FragPos = fbColor0.xyz;
  vec4 FragPosLightSpace = lightSpaceMatrix0 * vec4(FragPos, 1.0);
  vec3 normal = fbColor1.xyz;
  vec3 ambient = ambientLight * fbColor2.xyz;
  vec3 albedo = fbColor2.xyz * occlusion;
  vec3 specular_sample = fbColor3.xyz;
  float shininess = fbColor0.a * 100.f;

  //FragColor = vec4(ambient, 1.0);

  vec3 viewDir = normalize(viewPos - FragPos);

  vec3 diffuse = vec3(0.0, 0.0, 0.0);
  vec3 specular = vec3(0.0, 0.0, 0.0);

  float bias = 0.01;

  for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
    UPointLight iupl = upl[i];

    float distance = length(iupl.position - FragPos);
    // Unstable because uninitialized lights yield negative values. SHoud fix itself once we generate shader code
    float attenuation = 1.0 / (iupl.constant + iupl.linear * distance + iupl.quadratic * (distance * distance));

    vec3 lightDir = normalize(iupl.position - FragPos);
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuseContribution = attenuation * (iupl.diffuse * diffuseFactor * albedo);
    float shadow = (1 - computePointLightShadow(bias, FragPos));
    diffuse += shadow * (max(vec3(0.0), diffuseContribution));  // TODO: remove max after attenuation fix
    vec3 halfwayVec = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayVec), 0.0), shininess);
    vec3 specularContribution = attenuation * (iupl.specular * spec * specular_sample);
    specular += shadow * (max(vec3(0.0), specularContribution));  // TODO: remove max after attenuation fix
  }

  for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
    UDirectionLight iudl = udl[i];
    vec3 lightDir = normalize(-iudl.direction);
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    float shadow = (1 - computeShadow(bias, FragPosLightSpace));
    diffuse += shadow * (iudl.diffuse * diffuseFactor * albedo);
    vec3 halfwayVec = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayVec), 0.0), shininess);
    specular += shadow * (iudl.specular * spec * specular_sample);
  }

  vec3 result = diffuse + specular + (ambient * albedo);

  FragColor = vec4(result, 1.0);
  //FragColor = vec4(1.0, 0.0, 0.0, 1.0);

}
