#version 450 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D fb;

void main()
{ 
    vec4 fbColor1 = texture(fb, TexCoords);
    float gamma = 2.2;
    FragColor = vec4(pow(fbColor1.rgb, vec3(1.0/gamma)), 1.0);
}
