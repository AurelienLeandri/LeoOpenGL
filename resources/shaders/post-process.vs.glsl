#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = texCoords;
    gl_Position = vec4(position.x * 2.0 - 1.0, position.y * 2.0 - 1.0, 0.0, 1.0); 
} 
