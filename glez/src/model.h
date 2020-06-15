#pragma once

typedef struct
{
	f32 x, y, z;
} PosVertex;
typedef struct
{
	f32 vx, vy, vz, pad[1]; // pad[1];
	u8 nx, ny, nz, pad2[1]; //pad2[1];
	f32 tu, tv;
} PosNormTexVertex;

typedef struct
{
	PosVertex* vertices;
	u32* indices;
	u32 vertexCount, indexCount;
} SimpleMesh;

typedef struct
{
	PosNormTexVertex* vertices;
	u32* indices;
	u32 vertexCount, indexCount;
} Mesh;

Mesh Mesh_Load(const char* path);
