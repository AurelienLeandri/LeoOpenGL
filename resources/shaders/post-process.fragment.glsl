#version 450 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D fb0;

void main()
{ 
    vec4 fbColor1 = texture(fb0, TexCoords);
    FragColor = fbColor1;
}
