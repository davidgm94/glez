#include "page_allocator.h"
#include "lightwindows.h"
#include <assert.h>
#include <stdio.h>
#include "logger.h"

#define MAX_ALLOCS KILOBYTES(100)

typedef struct FreeNode
{
	void* allocatedMemory;
	size_t allocatedSize;
} FreeNode;
typedef struct FreeList
{
	FreeNode nodeList[MAX_ALLOCS];
	u64 allocationCount;
} FreeList;

FreeList i_FreeList = { 0 };

u32 i_PageSize = { 0 };

void memory_InitializePageAllocator(void)
{
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	u32 pageSize = systemInfo.dwPageSize;
	u32 granularity = systemInfo.dwAllocationGranularity;
	u32 resultingPageSize = pageSize > granularity ? pageSize : granularity;
	logInfo("[PAGE ALLOCATOR INITIALIZED] PAGE SIZE: %u bytes. GRANULARITY: %u bytes. RESULTING PAGE SIZE: %u bytes.\n", pageSize, granularity, resultingPageSize);
	i_PageSize = resultingPageSize;
}

static inline void* alignAddress(void* address, size_t align)
{
	const size_t mask = align - 1;
	uintptr_t p = (uintptr_t)address;
	assert((align & mask) == 0);

	return (void*)((p + mask) & ~mask);
}

static inline size_t roundUpToNextPage(size_t size)
{
	size_t remainder = (size % i_PageSize);
	size_t toAdd = i_PageSize - remainder;
	return size + toAdd;
}

AllocationResult allocateVirtualMemory(size_t size, size_t align)
{
	AllocationResult result = { 0 };
	assert(size > 0 && align > 0);
	if (size <= 0 || align <= 0) return result;

	size = roundUpToNextPage(size);

	s32 flags = MEM_RESERVE | MEM_COMMIT;
	void* p = VirtualAlloc(NULL, size, flags, PAGE_READWRITE);
	assert(p);
	if (p)
	{
		i_FreeList.nodeList[i_FreeList.allocationCount].allocatedMemory = p;
		i_FreeList.nodeList[i_FreeList.allocationCount].allocatedSize = size;
		++i_FreeList.allocationCount;
		logInfo("Reserving %u bytes of memory at address 0x%p\n", size, p);
		result.memory = alignAddress(p, align);
		const size_t lostMemory = ((uintptr_t)(result.memory) - (uintptr_t)p);
		result.size = size - lostMemory;
		logInfo("%u-byte aligned pointer: 0x%p. Lost memory: %zu\n", align, result.memory, lostMemory);
	}
	else
	{
		logError("Allocation failed!\n");
	}
	return result;
}

// TODO: fix this. Not working
void freeAllVirtualMemory(void)
{
	u64 allocationCount = i_FreeList.allocationCount;
	FreeNode* freeNodeList = i_FreeList.nodeList;

	for (u64 i = 0; i < allocationCount; i++)
	{
		FreeNode* node = &freeNodeList[i];
		BOOL result = VirtualFree(node->allocatedMemory, node->allocatedSize, 0);
		if (!result)
		{
			DWORD virtualFreeError = GetLastError();
			char errorBuffer[256];
			FormatMessageA(0, 0, virtualFreeError, 0, errorBuffer, sizeof(errorBuffer), 0);
			logError("VirtualFree failed: %s.\n", errorBuffer);
		}
	}
}
