#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coord;
out vec2 out_tex_coord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    //gl_Position = vec4(pos, 1.0f);
    gl_Position = proj * view * model * vec4(pos, 1.0f);
    out_tex_coord = vec2(tex_coord.x, tex_coord.y);
}
