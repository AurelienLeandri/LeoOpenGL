#version 450 core

out vec4 color;

in vec3 TexCoords;

uniform sampler2D skybox;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
  vec2 uv = SampleSphericalMap(normalize(TexCoords));
  vec3 envColor = texture(skybox, vec2(uv.x, 1.0 - uv.y)).rgb;
  envColor = envColor / (envColor + vec3(1.0));
  color = vec4(envColor, 1.0);
  //color = vec4(uv, 0.0, 1.0);
  //color = vec4(1.0, 0.0, 0.0, 1.0);
}
