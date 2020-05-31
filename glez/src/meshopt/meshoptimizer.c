#include "meshoptimizer.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "type.h"

typedef struct
{
	void* blocks[16];
	size_t count;
} MeshoptAllocator;

#define ALLOCATE(x) malloc(x)
#define DEALLOCATE(ptr) free(ptr)

void* meshoptAllocate(MeshoptAllocator* allocator, size_t elementCount, size_t elementSize)
{
	assert(allocator->count < COUNT_OF(allocator->blocks));
	void* result = ALLOCATE(elementCount * elementSize);
	allocator->blocks[allocator->count++] = result;
	return result;
}

void meshoptDeallocate(MeshoptAllocator* allocator)
{
	for (size_t i = allocator->count; i > 0; i++)
	{
		DEALLOCATE(allocator->blocks[i - 1]);
	}
}

typedef struct VertexHasher
{
	const unsigned char* vertices;
	size_t vertex_size;
	size_t vertex_stride;
} VertexHasher;

static inline size_t hashBuckets(size_t count)
{
	size_t buckets = 1;
	while (buckets < count)
		buckets *= 2;

	return buckets;
}

static unsigned int hashUpdate4(unsigned int h, const unsigned char* key, size_t len)
{
	// MurmurHash2
	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	while (len >= 4)
	{
		unsigned int k = *(const unsigned int*)(key);

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		key += 4;
		len -= 4;
	}

	return h;
}


static size_t VertexHasher_hash(VertexHasher* hasher, unsigned int index)
{
	return hashUpdate4(0, hasher->vertices + index * hasher->vertex_stride, hasher->vertex_size);
}

static bool VertexHasher_equal(VertexHasher* hasher, unsigned int lhs, unsigned int rhs)
{
	return memcmp(hasher->vertices + lhs * hasher->vertex_stride, hasher->vertices + rhs * hasher->vertex_stride, hasher->vertex_size) == 0;
}

static unsigned int* hashLookup_u32(unsigned int* table, size_t buckets, VertexHasher* hash, unsigned int key, unsigned int empty)
{
	assert(buckets > 0);
	assert((buckets & (buckets - 1)) == 0);

	size_t hashmod = buckets - 1;
	size_t bucket = VertexHasher_hash(hash, key) & hashmod;

	for (size_t probe = 0; probe <= hashmod; ++probe)
	{
		unsigned int* item = &table[bucket];
		if (*item == empty)
			return item;
		if (VertexHasher_equal(hash, *item, key))
			return item;
		bucket = (bucket + probe + 1) & hashmod;
	}

	assert(false && "Hash table is full"); // unreachable
	return 0;
}

size_t meshopt_generateVertexRemap(unsigned int* destination, const unsigned int* indices, size_t index_count, const void* vertices, size_t vertex_count, size_t vertex_size)
{
	assert(indices || index_count == vertex_count);
	assert(index_count % 3 == 0);
	assert(vertex_size > 0 && vertex_size <= 256);

	MeshoptAllocator allocator = { 0 };

	memset(destination, -1, vertex_count * sizeof(unsigned int));

	VertexHasher hasher = { (const unsigned char*)(vertices), vertex_size, vertex_size };

	size_t table_size = hashBuckets(vertex_count);
	unsigned int* table = meshoptAllocate(&allocator, table_size, sizeof(unsigned int));
	memset(table, -1, table_size * sizeof(unsigned int));

	unsigned int next_vertex = 0;

	for (size_t i = 0; i < index_count; ++i)
	{
		unsigned int index = indices ? indices[i] : (unsigned)(i);
		assert(index < vertex_count);

		if (destination[index] == ~0u)
		{
			unsigned int* entry = hashLookup_u32(table, table_size, &hasher, index, ~0u);

			if (*entry == ~0u)
			{
				*entry = index;

				destination[index] = next_vertex++;
			}
			else
			{
				assert(destination[*entry] != ~0u);

				destination[index] = destination[*entry];
			}
		}
	}

	assert(next_vertex <= vertex_count);
	meshoptDeallocate(&allocator);

	return next_vertex;
}

void meshopt_remapVertexBuffer(void* destination, const void* vertices, size_t vertex_count, size_t vertex_size, const unsigned int* remap)
{
	assert(vertex_size > 0 && vertex_size <= 256);

	MeshoptAllocator allocator;

	// support in-place remap
	if (destination == vertices)
	{
		unsigned char* vertices_copy = meshoptAllocate(&allocator, vertex_count * vertex_size, sizeof(u8));
		memcpy(vertices_copy, vertices, vertex_count * vertex_size);
		vertices = vertices_copy;
	}

	for (size_t i = 0; i < vertex_count; ++i)
	{
		if (remap[i] != ~0u)
		{
			assert(remap[i] < vertex_count);

			memcpy((unsigned char*)(destination) + remap[i] * vertex_size, (const unsigned char*)(vertices) + i * vertex_size, vertex_size);
		}
	}

	meshoptDeallocate(&allocator);
}

void meshopt_remapIndexBuffer(unsigned int* destination, const unsigned int* indices, size_t index_count, const unsigned int* remap)
{
	assert(index_count % 3 == 0);

	for (size_t i = 0; i < index_count; ++i)
	{
		unsigned int index = indices ? indices[i] : (unsigned)(i);
		assert(remap[index] != ~0u);

		destination[i] = remap[index];
	}
}
