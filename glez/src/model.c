#include "type.h"
#include "model.h"
#include "stb/stb_ds.h"
#include "meshopt/fast_obj.h"
#include <meshopt/meshoptimizer.h>
#include <assert.h>

Mesh Mesh_Load(const char* path)
{
	fastObjMesh* obj = fast_obj_read(path);
	assert(obj);

	u64 indexCount = 0;
	u32 face_count = obj->face_count;
	for (u32 i = 0; i < face_count; i++)
	{
		indexCount += 3 * (obj->face_vertices[i] - 2);
	}

	PosNormTexVertex* vertices = NULL;
	stbds_arrsetlen(vertices, indexCount);

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

	assert(vertex_offset == indexCount);
	fast_obj_destroy(obj);

	Mesh m = { 0 };
	u32* remap = NULL;
	stbds_arrsetlen(remap, indexCount);
	u64 vertexCount = meshopt_generateVertexRemap(remap, NULL, indexCount, vertices, indexCount, sizeof(PosNormTexVertex));

	stbds_arrsetlen(m.vertices, vertexCount);
	m.vertexCount = vertexCount;
	stbds_arrsetlen(m.indices, indexCount);
	m.indexCount = indexCount;

	meshopt_remapVertexBuffer(m.vertices, vertices, indexCount, sizeof(PosNormTexVertex), remap);
	meshopt_remapIndexBuffer(m.indices, NULL, indexCount, remap);
	
	stbds_arrfree(remap);
	stbds_arrfree(vertices);

	return m;
}
