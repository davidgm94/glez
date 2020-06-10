#pragma once
#include "type.h"

typedef struct AllocationResult
{
	void* memory;
	size_t size;
} AllocationResult;

void memory_InitializePageAllocator(void);

AllocationResult allocateVirtualMemory(size_t size, size_t align);

void freeAllVirtualMemory(void);

