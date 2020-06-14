#pragma once
#include "type.h"
#include "iallocator.h"

typedef struct FreeNode
{
	void* allocatedMemory;
	size_t allocatedSize;
} FreeNode;
#define PAGE_ALLOCATOR_MAX_ALLOCS KILOBYTES(100)
typedef struct PageAllocator
{
	IAllocator allocator;
	FreeNode* nodeList;
	u64 allocationCount;
	u64 pageSize;
	u64 maxAllocCount;
} PageAllocator;

PageAllocator* PageAllocator_Initialize(size_t maxAllocations);

AllocationResult allocateVirtualMemory(PageAllocator* allocator, size_t size, size_t align);

void freeAllVirtualMemory(PageAllocator* pageAllocator);

void PageAllocator_FreeAllPageAllocators(void);

