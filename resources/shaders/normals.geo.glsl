#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
  vec2 TexCoords;
  vec3 Normal;
  vec3 FragPos;
} gs_in[];

const float MAGNITUDE = 0.4;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

void GenerateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    Normal = gs_in[index].Normal;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].Normal, 0.0) * MAGNITUDE;
    Normal = gs_in[index].Normal;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}  
