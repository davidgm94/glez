#pragma once


#include "type.h"
#include <meshoptimizer.h>
#define FAST_OBJ_IMPLEMENTATION
#include <fast_obj.h>
#include <stb_ds.h>

typedef struct
{
	f32 vx, vy, vz, pad[1];
	u8 nx, ny, nz, pad2[1];
	f32 tu, tv;
} pos_norm_tex_vertex;

void load(const char* path)
{
	fastObjMesh* obj = fast_obj_read(path);

	u64 index_count = 0;
	u32 face_count = obj->face_count;
	for (u32 i = 0; i < face_count; i++)
	{
		index_count += 3 * (obj->face_vertices[i] - 2);
	}

	pos_norm_tex_vertex* vertices;
	stbds_arrsetlen(vertices, index_count);

	u64 vertex_offset = 0;
	u64 index_offset = 0;

	for (u32 i = 0; i < obj->face_count; i++)
	{
		for (u32 j = 0; j < obj->face_vertices[i]; j++)
		{
			fastObjIndex gi = obj->indices[index_offset + j];

			if (j >= 3)
			{
				vertices[vertex_offset + 0] = vertices[vertex_offset - 3];
				vertices[vertex_offset + 1] = vertices[vertex_offset - 1];
				vertex_offset += 2;
			}

			pos_norm_tex_vertex v;
			v.vx = obj->positions[gi.p * 3 + 0];
			v.vy = obj->positions[gi.p * 3 + 1];
			v.vz = obj->positions[gi.p * 3 + 2];

			v.nx = (u8)(obj->normals[gi.n * 3 + 0] * 127.f + 127.5f);
			v.ny = (u8)(obj->normals[gi.n * 3 + 1] * 127.f + 127.5f);
			v.nz = (u8)(obj->normals[gi.n * 3 + 2] * 127.f + 127.5f);

			v.tu = obj->texcoords[gi.t * 2 + 0];
			v.tv = obj->texcoords[gi.t * 2 + 1];

			vertices[vertex_offset] = v;
			vertex_offset++;
		} 
		index_offset += obj->face_vertices[i];
	}

	assert(vertex_offset == index_count);
	fast_obj_destroy(obj);
}