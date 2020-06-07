#include "type.h"
#include <string.h>

#define alignof(x) __alignof(x)
#define INLINE __forceinline

typedef struct
{
	void* p;
	size_t size;
} AllocationResult;

INLINE AllocationResult returnAllocationResult(void* p, size_t size)
{
	AllocationResult r = { .p = p, .size = size };
#ifdef GLEZ_DEBUG
	// TODO ???
	memset(r.p, 0x66, r.size);
#endif
	return r;
}

static const s32 DEFAULT_ALIGN = 4;

struct Allocator;
#define TYPEDEF_FN(fn)

#define TRACKED_ALLOCATION_FN(fn) AllocationResult fn(size_t size, u32 align)
#define ALLOCATION_WITH_ALIGN_FN(fn) void* fn(size_t size, u32 align)
#define ALLOCATION_FN(fn) void* fn(size_t size)
#define DEALLOCATE_FN(fn) size_t fn(void* p)
#define GET_ALLOCATED_SIZE_FN(fn) size_t fn(void* p)
#define TOTAL_ALLOCATED_SIZE_FN(fn) size_t fn(void)
#define GET_BACKING_ALLOCATORS_FN(fn) void fn(struct Allocator* allocators)
#define IS_TEMP_ALLOCATOR_FN(fn) void fn(void)

typedef TRACKED_ALLOCATION_FN(TrackedAllocationFn);
typedef ALLOCATION_WITH_ALIGN_FN(AllocationWithAlignFn);
typedef ALLOCATION_FN(AllocationFn);
typedef DEALLOCATE_FN(DeallocateFn);
typedef GET_ALLOCATED_SIZE_FN(GetAllocatedSizeFn);
typedef TOTAL_ALLOCATED_SIZE_FN(TotalAllocatedSizeFn);
typedef GET_BACKING_ALLOCATORS_FN(GetBackingAllocatorsFn);
typedef IS_TEMP_ALLOCATOR_FN(IsTempAllocatorFn);
#define NEW(a, T, size, align) // TODO: a.alloca
/*
	* Page allocator
	* Heap allocator
	* Slot allocator
	* Generic allocator
	* How do we allocate the allocators? -> Static heaps
	* Trace allocator -> CaptureStackBackTrace
*/

typedef struct Allocator
{
	TrackedAllocationFn* allocateWithResult;
	AllocationWithAlignFn* allocateNoAlign;
	AllocationFn* allocate;
	DeallocateFn* deallocate;
	GetAllocatedSizeFn* getAllocatedSize;
	TotalAllocatedSizeFn* getTotalAllocatedSize;
	GetBackingAllocatorsFn* getBackingAllocators;
	IsTempAllocatorFn* isTempAllocator;
} Allocator;
void allocate(void);

size_t roundUpToPageSize(size_t size, size_t pageSize);

void* pointerAdd(void* p1, u32 bytes);

void* pointerSub(void* p1, u32 bytes);

void* roundDownToAlignAdress(void* ptr, u32 align);
