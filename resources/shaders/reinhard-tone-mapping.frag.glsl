#version 450 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D fb;

void main()
{ 
    vec3 fbColor1 = texture(fb, TexCoords).xyz;

    // reinhard tone mapping
    //float exposure = 1.0;
    //vec3 mapped = vec3(1.0) - exp(-fbColor1 * exposure);
    vec3 mapped = fbColor1 / (fbColor1 + vec3(1.0));

    FragColor = vec4(mapped, 1.0);
}
