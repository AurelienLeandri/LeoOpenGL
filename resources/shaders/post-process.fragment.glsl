#version 450 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D fb;

void main()
{ 
    vec4 fbColor1 = texture(fb, TexCoords);
    FragColor = vec4(fbColor1.xyz, 1.0);
}
