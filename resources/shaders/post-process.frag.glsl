#version 450 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D fb0;
uniform sampler2D fb1;

void main()
{ 
    vec4 fbColor0 = texture(fb0, TexCoords);
    vec4 fbColor1 = texture(fb1, TexCoords);
    FragColor = vec4(fbColor0.xyz, 1.0);
}
