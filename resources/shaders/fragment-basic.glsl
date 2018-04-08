#version 450

out vec4 color;

in vec3 fragNormal;
in vec3 fragPos;
in vec2 vUv;

void main() {
    color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
