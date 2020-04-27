#version 330 core
layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 normals;
layout (location = 2) in vec2 tex_coords;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec2 tex_coord;

void main()
{
    gl_Position = proj * view * model * pos;
    tex_coord = vec2(tex_coords.x, tex_coords.y);
}