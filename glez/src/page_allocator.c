#include "page_allocator.h"
#include "lightwindows.h"
#include <assert.h>
#include <stdio.h>
#include "logger.h"
#include "iallocator.h"

#define MAX_PAGE_ALLOCATOR_COUNT 4
static PageAllocator* pageAllocators[MAX_PAGE_ALLOCATOR_COUNT];
static u32 pageAllocatorSize[MAX_PAGE_ALLOCATOR_COUNT];
static u32 pageAllocatorCount = 0;
#define PAGE_ALLOCATOR_ADDRESS (void*)(MEGABYTES(2)) 
#define PAGE_ALLOCATOR_SIZE(MAX_ALLOCS) (sizeof(PageAllocator) + (sizeof(FreeNode) * MAX_ALLOCS))
PageAllocator* PageAllocator_Initialize(size_t maxAllocations)
{
	u32 allocatorIndex = pageAllocatorCount++;
	const u32 PageAllocatorSize = pageAllocatorSize[allocatorIndex] = PAGE_ALLOCATOR_SIZE(maxAllocations);
	PageAllocator* pageAllocator = NULL;
	logInfo("Asking %zu bytes for page allocator at address 0x%p\n", PageAllocatorSize, PAGE_ALLOCATOR_ADDRESS);
	pageAllocator = VirtualAlloc(PAGE_ALLOCATOR_ADDRESS, PageAllocatorSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	assert(pageAllocator);
	assert(pageAllocator == PAGE_ALLOCATOR_ADDRESS);
	uintptr_t pageAllocatorPtr = (uintptr_t)pageAllocator;
	// MANDATORY: Page Allocator is 4-byte aligned
	assert(!(sizeof(PageAllocator) % 4 && !(pageAllocatorPtr % 4)));
	pageAllocator->nodeList = (FreeNode*)(pageAllocatorPtr + sizeof(PageAllocator));
	pageAllocator->maxAllocCount = maxAllocations;
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	u32 resultingPageSize = systemInfo.dwPageSize > systemInfo.dwAllocationGranularity ? systemInfo.dwPageSize : systemInfo.dwAllocationGranularity;
	pageAllocator->pageSize = resultingPageSize;
	pageAllocator->allocator.allocateWithResult = (TrackedAllocationFn*) allocateVirtualMemory;
	logInfo("Page Allocator initialized at address 0x%p with page size of %zu bytes\n", pageAllocator, resultingPageSize);
	if (!pageAllocator)
	{
		pageAllocatorSize[--pageAllocatorCount] = 0;
	}
	else
	{
		pageAllocators[allocatorIndex] = pageAllocator;
	}

	return pageAllocator;
}

static inline void* alignAddress(void* address, size_t align)
{
	const size_t mask = align - 1;
	uintptr_t p = (uintptr_t)address;
	assert((align & mask) == 0);

	return (void*)((p + mask) & ~mask);
}

static inline size_t roundUpToNextPage(size_t size, size_t pageSize)
{
	size_t remainder = (size % pageSize);
	size_t toAdd = pageSize - remainder;
	return size + toAdd;
}

AllocationResult allocateVirtualMemory(PageAllocator* pageAllocator, size_t size, size_t align)
{
	AllocationResult result = { 0 };
	assert(size > 0 && align > 0);
	if (size <= 0 || align <= 0) return result;

	size = roundUpToNextPage(size, pageAllocator->pageSize);

	s32 flags = MEM_RESERVE | MEM_COMMIT;
	void* p = VirtualAlloc(NULL, size, flags, PAGE_READWRITE);
	assert(p);
	if (p)
	{
		pageAllocator->nodeList[pageAllocator->allocationCount].allocatedMemory = p;
		pageAllocator->nodeList[pageAllocator->allocationCount].allocatedSize = size;
		++pageAllocator->allocationCount;
		logInfo("Reserving %u bytes of memory at address 0x%p\n", size, p);
		result.p = alignAddress(p, align);
		const size_t lostMemory = ((uintptr_t)(result.p) - (uintptr_t)p);
		result.size = size - lostMemory;
		logInfo("%u-byte aligned pointer: 0x%p. Lost memory: %zu\n", align, result.p, lostMemory);
	}
	else
	{
		logError("Allocation failed!\n");
	}
	return result;
}

// TODO: fix this. Not working
void freeAllVirtualMemory(PageAllocator* pageAllocator)
{
	u64 allocationCount = pageAllocator->allocationCount;
	FreeNode* freeNodeList = pageAllocator->nodeList;

	for (u64 i = 0; i < allocationCount; i++)
	{
		FreeNode node = freeNodeList[i];
		// Currently returning error
		BOOL result = VirtualFree(node.allocatedMemory, node.allocatedSize, 0);
		if (!result)
		{
			// Can't get error
			DWORD virtualFreeError = GetLastError();
			char errorBuffer[256];
			FormatMessageA(0, 0, virtualFreeError, 0, errorBuffer, sizeof(errorBuffer), 0);
			logError("VirtualFree failed for address 0x%p (size: %zu bytes): %s.\n", node.allocatedMemory, node.allocatedSize, errorBuffer);
		}
	}
}

void PageAllocator_FreeAllPageAllocators(void)
{
	for (s32 i = 0; i < pageAllocatorCount; i++)
	{
		// TODO: VirtualFree is not working. Find out why
		if (VirtualFree(pageAllocators + i, pageAllocatorSize[i], MEM_DECOMMIT | MEM_RELEASE))
		{
			logInfo("Freed page allocator %u at address 0x%p.\n", i, pageAllocators + i);
		}
		else
		{
			logError("Failing to free page allocator %u at address 0x%p.\n", i, pageAllocators + i);
		}
	}
}
