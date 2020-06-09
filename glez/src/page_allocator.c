#include "page_allocator.h"
#include "lightwindows.h"
#include <assert.h>
#include <stdio.h>
#include "logger.h"

static inline size_t getPageSize(void)
{
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	return systemInfo.dwPageSize;
}

static inline size_t getGranularity(void)
{
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	return systemInfo.dwAllocationGranularity;
}

AllocationResult reserveVirtualMemory(size_t size, size_t align, bool commit)
{
	AllocationResult result = { 0 };
	assert(size > 0 && align > 0);
	if (size <= 0 || align <= 0) return result;

	s32 flags = MEM_RESERVE;
	if (commit) flags |= MEM_COMMIT;
	void* p = VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_READWRITE);
	assert(p);
	if (p)
	{
		result.p = p;
		result.size = size;
		logInfo("Reserving %u bytes of memory at address 0x%p. [%s]\n", result.size, result.p, commit ? "committing" : "no committing");
	}
	return result;
}

void* commitMemory(AllocationResult* allocation)
{
	if (allocation->p)
	{
		void* p = VirtualAlloc(allocation->p, allocation->size, MEM_COMMIT, PAGE_READWRITE);
		assert(p);
		allocation->p = p;
		float* fp = p;
		*fp = 0.0f;
	}
	else
	{
		allocation->p = NULL;
		allocation->size = 0;
	}
	return allocation->p;
}


void* allocate(size_t size, size_t align)
{
	void* address = NULL;
	void* p = VirtualAlloc(address, size, MEM_RESERVE, PAGE_READWRITE);
	if (!p)
	{
		DWORD lastError = GetLastError();
		char buffer[256];
		FormatMessageA(0, allocate, lastError, NULL, buffer, sizeof(buffer), NULL);
		OutputDebugStringA("\n");
		OutputDebugStringA(buffer);
		OutputDebugStringA("\n");
		OutputDebugStringA("\n");
		//assert(p);
	}
	return p;
}
