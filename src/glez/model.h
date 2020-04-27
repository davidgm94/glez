#pragma once

typedef struct
{
	f32 vx, vy, vz, pad[1]; // pad[1];
	u8 nx, ny, nz, pad2[1]; //pad2[1];
	f32 tu, tv;
} pos_norm_tex_vertex;

typedef struct
{
	pos_norm_tex_vertex* vertices;
	u32* indices;
	u32 vertex_count, index_count;
} mesh;

mesh load_mesh(const char* path);
