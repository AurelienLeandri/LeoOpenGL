#version 450 core

#define MAX_NUM_LIGHTS 10

out vec4 FragColor;
  
in vec2 TexCoords;

// GBuffer
uniform sampler2D fb0;
uniform sampler2D fb1;
uniform sampler2D fb2;
uniform sampler2D fb3;

void main()
{ 
  vec4 fbColor0 = texture(fb0, TexCoords);
  vec4 fbColor1 = texture(fb1, TexCoords);
  vec4 fbColor2 = texture(fb2, TexCoords);
  vec4 fbColor3 = texture(fb3, TexCoords);

  vec3 FragPos = fbColor0.xyz;
  vec3 normal = fbColor1.xyz;
  vec3 albedo = fbColor2.xyz;
  vec3 specular_sample = fbColor3.xyz;

  FragColor = vec4(normal, 1.0);
}
