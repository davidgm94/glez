#include "type.h"
#include "model.h"
#include "stb/stb_ds.h"
#include "meshopt/fast_obj.h"
#include <meshopt/meshoptimizer.h>
#include <assert.h>

Mesh meshLoad(const char* path)
{
	fastObjMesh* obj = fast_obj_read(path);
	assert(obj);

	u64 index_count = 0;
	u32 face_count = obj->face_count;
	for (u32 i = 0; i < face_count; i++)
	{
		index_count += 3 * (obj->face_vertices[i] - 2);
	}

	PosNormTexVertex* vertices = NULL;
	stbds_arrsetlen(vertices, index_count);

	u64 vertex_offset = 0;
	u64 index_offset = 0;

	for (u32 i = 0; i < face_count; i++)
	{
		u32 face_vertices = obj->face_vertices[i];
		for (u32 j = 0; j < face_vertices; j++)
		{
			fastObjIndex gi = obj->indices[index_offset + j];

			if (j >= 3)
			{
				vertices[vertex_offset + 0] = vertices[vertex_offset - 3];
				vertices[vertex_offset + 1] = vertices[vertex_offset - 1];
				vertex_offset += 2;
			}

			PosNormTexVertex v;
			v.vx = obj->positions[gi.p * 3 + 0];
			v.vy = obj->positions[gi.p * 3 + 1];
			v.vz = obj->positions[gi.p * 3 + 2];
			v.pad[0] = 1.0f;

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

	Mesh m = { 0 };
	u32* remap = NULL;
	stbds_arrsetlen(remap, index_count);
	u64 vertex_count = meshopt_generateVertexRemap(remap, NULL, index_count, vertices, index_count, sizeof(PosNormTexVertex));

	stbds_arrsetlen(m.vertices, vertex_count);
	m.vertex_count = vertex_count;
	stbds_arrsetlen(m.indices, index_count);
	m.index_count = index_count;

	meshopt_remapVertexBuffer(m.vertices, vertices, index_count, sizeof(PosNormTexVertex), remap);
	meshopt_remapIndexBuffer(m.indices, NULL, index_count, remap);
	
	stbds_arrfree(remap);
	stbds_arrfree(vertices);

	return m;
}
