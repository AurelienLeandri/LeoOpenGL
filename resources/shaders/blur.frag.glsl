#version 450 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D fb;
uniform bool horizontal;

uniform float gaussian_weights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 tex_offset = 1.0 / textureSize(fb, 0); // gets size of single texel
    vec3 result = texture(fb, TexCoords).rgb * gaussian_weights[0]; // current fragment's contribution
    if (horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(fb, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * gaussian_weights[i];
            result += texture(fb, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * gaussian_weights[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(fb, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * gaussian_weights[i];
            result += texture(fb, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * gaussian_weights[i];
        }
    }
    FragColor = vec4(result, 1.0);
}
