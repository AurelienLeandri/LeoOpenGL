
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;

out vec3 localPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    localPos = position;  
    gl_Position =  projection * view * vec4(localPos, 1.0);
}