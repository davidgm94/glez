#pragma once

#include "type.h"

typedef struct LinearAllocator
{
	char* _start, * _end, * _current;
} LinearAllocator;

void* LinearAllocator_Allocate(LinearAllocator* allocator, size_t size, size_t alignment, size_t offset);
void LinearAllocator_Free(LinearAllocator* allocator, void* ptr);
void LinearAllocator_Reset(LinearAllocator* allocator);