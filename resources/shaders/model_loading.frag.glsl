#version 450 core

in vec2 TexCoords;
in vec3 Normal;

out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{
    color = vec4(texture(texture_diffuse1, TexCoords));
    //color = vec4(TexCoords, 0.0, 1.0);
    //color = vec4(Normal, 1.0);
}
