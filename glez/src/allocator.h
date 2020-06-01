#include "type.h"
typedef void* AllocateFn(size_t size, size_t align);
typedef void DeallocateFn(void* p);
typedef size_t GetAllocatedSizeFn(void* p);

typedef struct Allocator
{
	AllocateFn* allocate;
	DeallocateFn* deallocate;
	GetAllocatedSizeFn* getAllocateSize;
} Allocator;
void allocate(void);