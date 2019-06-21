#version 450 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D fb0;

void main()
{ 
    vec3 fbColor0 = texture(fb0, TexCoords).xyz;


    // reinhard tone mapping
    float exposure = 0.5;
    vec3 mapped = vec3(1.0) - exp(-fbColor0 * exposure);

    FragColor = vec4(fbColor0, 1.0);
}
