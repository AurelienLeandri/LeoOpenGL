#version 450 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D fb0;
uniform sampler2D fb1;

void main()
{ 
    vec4 fbColor1 = texture(fb0, TexCoords) * vec4(0.0, 1.0, 0.0, 1.0);
    vec4 fbColor2 = texture(fb1, TexCoords) * vec4(1.0, 0.0, 0.0, 1.0);
    FragColor = mix(fbColor1, fbColor2, 0.5f);
}
