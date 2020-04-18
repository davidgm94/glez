#version 330 core
out vec4 output_color;
in vec3 frag_color;
void main()
{
    output_color = vec4(frag_color, 1.0);
}