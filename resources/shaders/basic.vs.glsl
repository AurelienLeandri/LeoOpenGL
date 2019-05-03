#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;
out vec3 Tangent;
out vec3 BiTangent;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix0;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoords = texCoords;
    FragPos = vec3(model * vec4(position, 1.0));
    FragPosLightSpace = lightSpaceMatrix0 * vec4(FragPos, 1.0);
    Normal = normalize(mat3(transpose(inverse(model))) * normal);
    BiTangent = normalize(mat3(transpose(inverse(model))) * biTangent);

    vec3 in_tangent = normalize(mat3(transpose(inverse(model))) * tangent);

    // Graham Schmidt (TODO: review)
    in_tangent = normalize(in_tangent - dot(in_tangent, Normal) * Normal);
    BiTangent = cross(Normal, in_tangent);

    if (dot(cross(in_tangent, BiTangent), Normal) < 0.0) {
        Tangent = -in_tangent;
    }
    else {
        Tangent = in_tangent;
    }
    TBN = mat3(Tangent, BiTangent, Normal);
}
