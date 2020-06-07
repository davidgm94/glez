#include "allocator.h"

#include <math.h>
void allocate(void)
{
}

// TODO: optimize algorithm
size_t roundUpToPageSize(size_t size, size_t pageSize)
{
#if 1
	size_t value = (size_t)-1;
	for (s32 i = 0; size > value; i++)
	{
		value = (size_t)pow(2, i);
	}
	return value;
#else
#endif
}

void* pointerAdd(void* p, u32 increment)
{
	return (void*)((char*)p + increment);
}

void* pointerSub(void* p, u32 increment)
{
	return (void*)((char*)p - increment);
}

void* roundDownToAlignAdress(void* p, u32 align)
{
	uintptr_t pi = (uintptr_t)p;
	const u32 mod = pi % align;
	if (mod)
		pi += (align - mod);
	return (void*)pi;
}
