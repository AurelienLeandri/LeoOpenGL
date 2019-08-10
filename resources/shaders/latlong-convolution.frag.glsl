#version 450 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D latlong;

void main()
{ 
    vec3 color = texture(latlong, vec2(TexCoords.x, TexCoords.y)).xyz;
    FragColor = vec4(color.x, 0.0, 0.0, 1.0);
}
