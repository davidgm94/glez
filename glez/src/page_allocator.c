#include "page_allocator.h"
#include "allocator.h"
#include <assert.h>

#define ALLOCATE_CANARY_PAGE 1

size_t pageSize(void);
size_t getPageSize(void);
void* allocatePages(size_t size, size_t align);
#if GLEZ_DEBUG
void* allocate_DebugMemory(size_t size);
#endif
void deallocatePages(void* p);
size_t allocatedSize(void* p);

static size_t _total = 0;
static size_t _pageSize = 0;

size_t roundUpToPageSize(size_t size);
void* pointerAdd(void* p1, void* p2);
void* pointerSub(void* p1, void* p2);
void* roundDownToAlignAdress(void* ptr, u32 align);

Allocator* pageAllocator = NULL;

TRACKED_ALLOCATION_FN(Page_AllocateWithResult)
{
	// Critical section
	size_t virtualMemorySize = roundUpToPageSize(size);
	void* p = allocatePages(virtualMemorySize, roundUpToPageSize((size_t)align));
	assert(p);

#if ALLOCATE_CANARY_PAGE
	AllocationResult r;
	r.p = roundDownToAlignAdress(pointerAdd(p, (void*)virtualMemorySize - size), align);
	r.size = (size_t)pointerSub(pointerAdd(p, (void*)virtualMemorySize), r.p);
	_total += r.size;
	return r;
#else
	size_t s = allocatedSize(p);
	_total += s;
	return pageAllocator->allocateWithResult(p, s);
#endif
}

DEALLOCATE_FN(Page_Deallocate)
{
	if (p == 0)
		return 0;

	// Critical section
	size_t s = allocatedSize(p);
	_total += s;

#if ALLOCATE_CANARY_PAGE
	p = roundDownToAlignAdress(p, pageSize());
#endif

	deallocatePages(p);
	return s;
}

GET_ALLOCATED_SIZE_FN(Page_GetAllocatedSize)
{
	// Critical section
#if ALLOCATE_CANARY_PAGE
	void* virtualMemoryPtr = roundDownToAlignAdress(p, pageSize());
	size_t virtualMemorySize = allocatedSize(virtualMemoryPtr);
	return (size_t)pointerSub(pointerAdd(virtualMemoryPtr, (void*)virtualMemorySize), p);
#else
	return allocatedSize(p);
#endif
}

#if GLEZ_DEBUG
void* Page_Allocate_DebugMemory(size_t size)
{
	void* p = allocate_DebugMemory(size);
	assert(p);
	return p;
}
#endif

#include <lightwindows.h>
size_t pageSize(void)
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	DWORD granularity = si.dwAllocationGranularity;
	if (si.dwPageSize > granularity)
		granularity = si.dwPageSize;
	return granularity;
}

size_t getPageSize(void)
{
	return _pageSize;
}

void* allocatePages(size_t size, size_t align)
{
	assert(align <= pageSize());
#if ALLOCATE_CANARY_PAGE
	size_t pageSize = getPageSize();
	void* p = VirtualAlloc(NULL, size + pageSize, MEM_COMMIT, PAGE_READWRITE);
	VirtualAlloc(pointerAdd(p, (void*)size), pageSize, MEM_COMMIT, PAGE_NOACCESS);
	return p;
#else
	return VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
#endif
}

#if GLEZ_DEBUG
void* allocate_DebugMemory(size_t size)
{
#if ALLOCATE_CANARY_PAGE
	size_t pageSize = getPageSize();
	void* p = VirtualAlloc(NULL, size + pageSize, MEM_COMMIT, PAGE_READWRITE);
	VirtualAlloc(pointerAdd(p, (void*)size), pageSize, MEM_COMMIT, PAGE_NOACCESS);
	return p;
#else
	return VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
#endif
}
#endif

void deallocatePages(void* p)
{
	s32 result = VirtualFree(p, 0, MEM_RELEASE);
	assert(result);
}

size_t allocatedSize(void* p)
{
	MEMORY_BASIC_INFORMATION info;
	VirtualQuery(p, &info, sizeof(info));
	return info.RegionSize;
}
