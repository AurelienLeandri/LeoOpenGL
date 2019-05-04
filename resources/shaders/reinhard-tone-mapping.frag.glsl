#version 450 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D fb;

void main()
{ 
    vec3 fbColor1 = texture(fb, TexCoords).xyz;

    // reinhard tone mapping
    float exposure = 0.5;
    vec3 mapped = vec3(1.0) - exp(-fbColor1 * exposure);

    FragColor = vec4(mapped, 1.0);
}
