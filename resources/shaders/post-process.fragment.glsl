#version 450 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D screenTexture2;

void main()
{ 
    vec4 fbColor = texture(screenTexture, TexCoords);
    vec4 fbColor2 = texture(screenTexture2, TexCoords);
    float bnw = (fbColor.x + fbColor.y + fbColor.z) / 3.0;
    float bnw2 = (fbColor2.x + fbColor2.y + fbColor2.z) / 3.0;
    //FragColor = vec4(bnw * bnw2, bnw * bnw2, bnw * bnw2, 1.0);
    FragColor = vec4(bnw, bnw, bnw, 1.0);
    //FragColor = vec4(TexCoords.x, TexCoords.y, bnw, 1.0);
    //FragColor = vec4(fbColor);
}
