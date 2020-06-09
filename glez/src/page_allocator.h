#pragma once
#include "type.h"

typedef struct AllocationResult
{
	void* p;
	size_t size;
} AllocationResult;

AllocationResult reserveVirtualMemory(size_t size, size_t align, bool commit);

void* commitMemory(AllocationResult* allocation);

void* allocate(size_t size, size_t align);
