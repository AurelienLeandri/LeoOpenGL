#version 450 core

out vec4 color;

void main()
{
  float near = 0.1; 
  float far  = 100.0;  // All values are above 1 after this value
  float depth = gl_FragCoord.z;
  float z = depth * 2.0 - 1.0; 
  float linearDepth = (2.0 * near * far) / (far + near - z * (far - near)); 
  color = vec4(vec3(linearDepth / far), 1.0);
}
