#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix0;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoords = texCoords;
    FragPos = vec3(model * vec4(position, 1.0));
    FragPosLightSpace = lightSpaceMatrix0 * vec4(FragPos, 1.0);
    Normal = mat3(transpose(inverse(model))) * normal;
}
