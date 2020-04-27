#version 330 core

layout (location = 0) vec4 pos;
layout (location = 1) vec4 normals;
layout (location = 2) vec2 tex_coords;

uniform vec4 quat;
uniform vec3 t;
uniform float s;

out vec2 tex_coord;
void main()
{
	vec3 view_position = vec3(
		dot(pos, model_view[0]), 
		dot(pos, model_view[1]),
		dot(pos, model_view[2])
		);
	
	gl_Position.x = view_position.x * projection.x;
	gl_Position.y = view_position.y * projection.y;
	gl_Position.z = view_position.z * projection.z + projection.w;
	gl_Position.w = -view_position.z;

	tex_coord = vec2(tex_coords.x, tex_coords.y);
}
