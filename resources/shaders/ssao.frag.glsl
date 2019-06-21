#version 450 core

#define MAX_NUM_LIGHTS 10

out vec4 FragColor;
  
in vec2 TexCoords;

// GBuffer
uniform sampler2D fb0;
uniform sampler2D fb1;

uniform mat4 projection;

uniform sampler2D noise;

uniform vec3 kernel[64];

float radius = 4.0;
int kernelSize = 64;
float bias = 4.0;

void main()
{
  const vec2 noiseScale = vec2(1620.0/4.0, 1080.0/4.0);
  vec4 fbColor0 = texture(fb0, TexCoords);
  vec4 fbColor1 = texture(fb1, TexCoords);
  vec3 noiseValue = normalize(texture(noise, TexCoords * noiseScale).xyz);

  float depth = fbColor0.z;

  vec3 normal = normalize(fbColor1.xyz);

  vec3 tangent   = normalize(noiseValue - normal * dot(noiseValue, normal));
  vec3 bitangent = cross(normal, tangent);
  mat3 TBN       = mat3(tangent, bitangent, normal);

  vec3 FragPos = fbColor0.xyz;

  vec3 red = vec3(1.0, 1.0, 1.0);

  float occlusion = 0.0;
  float n_radius = radius;
  for (int i = 0; i < kernelSize; i++) {
      vec3 s = (kernel[i]) * n_radius + FragPos;
      vec4 offset = vec4(s, 1.0);
      offset = projection * offset;    // from view to clip-space
      offset.xyz /= offset.w;               // perspective divide
      offset.xyz  = offset.xyz * 0.5 + 0.5;
      if (offset.x < 0.0 || offset.y < 0.0) {
        red = vec3(1.0, 0.0, 0.0);
      }
      if (offset.x > 1.0 || offset.y > 1.0) {
        red = vec3(1.0, 0.0, 0.0);
      }
      float sampleDepth = texture(fb0, offset.xy).z;
      float rangeCheck = smoothstep(0.0, 1.0, n_radius / abs(FragPos.z - sampleDepth));
      occlusion += (sampleDepth >= s.z + bias ? 1.0 : 0.0) * rangeCheck; 
      //occlusion += (sampleDepth >= s.z + bias ? 1.0 : 0.0); 

  }
  occlusion = 1.0 - (occlusion / kernelSize);
   
  vec3 total = vec3(occlusion) * red;
  FragColor = vec4(vec3(occlusion), 1.0);
  //FragColor = vec4(total, 1.0);
  //FragColor = vec4(vec3(projection * vec4(FragPos + kernel[0], 1.0)), 1.0);
  //FragColor = vec4(fbColor0.xy, -fbColor0.z, 1.0);
  //FragColor = vec4(vec3(fbColor0.x), 1.0);
  //FragColor = vec4(vec3(projection * vec4(fbColor0.xyz, 1.0)), 1.0);

}
