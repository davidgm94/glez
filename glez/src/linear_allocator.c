#include "linear_allocator.h"

void* LinearAllocator_Allocate(LinearAllocator* allocator, size_t size, size_t alignment, size_t offset)
{
	return NULL;
}

void LinearAllocator_Free(LinearAllocator* allocator, void* ptr)
{
	_alloca(0);
}

void LinearAllocator_Reset(LinearAllocator* allocator)
{
}
