#version 450 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    FragColor = texture(screenTexture, TexCoords);
    //FragColor = vec4(TexCoords.x, TexCoords.y, 0.0, 1.0);
}
