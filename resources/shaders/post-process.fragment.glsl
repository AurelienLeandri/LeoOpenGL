#version 450 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    vec4 fbColor = texture(screenTexture, TexCoords);
    float bnw = (fbColor.x + fbColor.y + fbColor.z) / 3.0;
    FragColor = vec4(bnw, bnw, bnw, 1.0);
    //FragColor = vec4(fbColor);
}
