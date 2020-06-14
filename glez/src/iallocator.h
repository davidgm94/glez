#pragma once
#include "type.h"
#include <string.h>

#define alignof(x) __alignof(x)
#define INLINE __forceinline
extern double pow(double x, double y);

typedef struct AllocationResult
{
	void* p;
	size_t size;
} AllocationResult;

static const s32 DEFAULT_ALIGN = 4;

typedef struct IAllocator IAllocator;
#define TYPEDEF_FN(fn)

#define TRACKED_ALLOCATION_FN(fn) AllocationResult fn(IAllocator* allocator, size_t size, size_t align)
#define ALLOCATION_WITH_ALIGN_FN(fn) void* fn(size_t size, size_t align)
#define ALLOCATION_FN(fn) void* fn(size_t size)
#define DEALLOCATE_FN(fn) size_t fn(void* p)
#define GET_ALLOCATED_SIZE_FN(fn) size_t fn(void* p)
#define TOTAL_ALLOCATED_SIZE_FN(fn) size_t fn(void)
#define GET_BACKING_ALLOCATORS_FN(fn) void fn(IAllocator* allocators)
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

typedef struct IAllocator
{
	TrackedAllocationFn* allocateWithResult;
	AllocationWithAlignFn* allocateNoAlign;
	AllocationFn* allocate;
	DeallocateFn* deallocate;
	GetAllocatedSizeFn* getAllocatedSize;
	TotalAllocatedSizeFn* getTotalAllocatedSize;
	GetBackingAllocatorsFn* getBackingAllocators;
	IsTempAllocatorFn* isTempAllocator;
} IAllocator;

// TODO: Optimize algorithm
inline size_t roundUpToPageSize(size_t size, size_t pageSize)
{
#if 1
	size_t value = 0;
	for (s32 i = 0; size > value; i++)
	{
		value = (size_t)pow(2, i);
	}
	// assert(value != (size_t)-1);
	return value;
#else
#endif
}

inline void* pointerAdd(void* p, u32 increment)
{
	return (void*)((char*)p + increment);
}

inline void* pointerSub(void* p, u32 increment)
{
	return (void*)((char*)p - increment);
}

inline void* roundDownToAlignAdress(void* p, u32 align)
{
	uintptr_t pi = (uintptr_t)p;
	const u32 mod = pi % align;
	if (mod)
		pi += (align - mod);
	return (void*)pi;
}
//INLINE AllocationResult returnAllocationResult(void* p, size_t size)
//{
//	AllocationResult r = { .p = p, .size = size };
//#ifdef GLEZ_DEBUG
//	// TODO ???
//	memset(r.p, 0x66, r.size);
//#endif
//	return r;
//}
//
