#version 330 core
in vec2 out_tex_coord;
uniform sampler2D tex;
out vec4 out_frag_color;

void main()
{
    out_frag_color = texture(tex, out_tex_coord);
}