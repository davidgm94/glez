#version 330 core
out vec4 out_frag_color;
in vec2 out_tex_coord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    out_frag_color = mix(texture(texture1, out_tex_coord), texture(texture2, out_tex_coord), 0.2);
}
