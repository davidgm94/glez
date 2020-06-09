#pragma once
#include "type.h"
#include "allocator.h"

#define PAGE_ALLOCATOR_PAGE_SIZE_FN(fn) size_t fn(void)
typedef PAGE_ALLOCATOR_PAGE_SIZE_FN(PageAllocator_PageSizeFn);
#define PAGE_ALLOCATOR_ALLOCATE_PAGES_FN(fn) void* fn(size_t size, size_t align)
typedef PAGE_ALLOCATOR_ALLOCATE_PAGES_FN(PageAllocator_AllocatePagesFn);
#if GLEZ_DEBUG
#define PAGE_ALLOCATOR_ALLOCATE_DEBUG_MEMORY_FN(fn) void* fn(size_t size)
typedef PAGE_ALLOCATOR_ALLOCATE_DEBUG_MEMORY_FN(PageAllocator_AllocateDebugMemoryFn);
#endif
#define PAGE_ALLOCATOR_DEALLOCATE_PAGES_FN(fn) void fn(void* p)
typedef PAGE_ALLOCATOR_DEALLOCATE_PAGES_FN(PageAllocator_DeallocatePagesFn);
#define PAGE_ALLOCATOR_ALLOCATED_SIZE_FN(fn) size_t fn(void* p)
typedef PAGE_ALLOCATOR_ALLOCATED_SIZE_FN(PageAllocator_AllocatedSizeFn);

typedef struct PageAllocator
{
	PageAllocator_PageSizeFn* pageSize;
	PageAllocator_AllocatePagesFn* allocatePages;
#if GLEZ_DEBUG
	PageAllocator_AllocateDebugMemoryFn* allocateDebugMemory;
#endif
	PageAllocator_DeallocatePagesFn* deallocatePages;
	PageAllocator_AllocatedSizeFn* allocatedSize;
} PageAllocator;

Allocator* memory_initPageAllocator(void);
