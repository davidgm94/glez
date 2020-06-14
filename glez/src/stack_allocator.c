#include "type.h"
#include "stack_allocator.h"
#include "iallocator.h"
#include "logger.h"
#include <assert.h>

void StackAllocator_Init(StackAllocator* stackAllocator, size_t requestedSize, IAllocator* backingAllocator)
{
	memset(stackAllocator, 0, sizeof(StackAllocator));
	assert(requestedSize % 4 == 0);
	AllocationResult result = backingAllocator->allocateWithResult(backingAllocator, requestedSize, 4);
	stackAllocator->_backingAllocator = backingAllocator;
	stackAllocator->stackPointer = stackAllocator->basePointer = result.p;
	stackAllocator->totalSize = result.size;
}

void* StackAllocator_Allocate(StackAllocator* stackAllocator, size_t size, size_t align)
{
	void* stackPtr = stackAllocator->stackPointer;
	u32 index = stackAllocator->allocationCount;
	void* ptr = stackPtr + size;
	ptr = roundDownToAlignAdress(ptr, align);
	size_t allocatedSize = ptr - stackPtr;
	AllocationResult result;
	result.size = allocatedSize;
	stackAllocator->allocations[index] = ptr;
	stackAllocator->allocationsSize[index] = allocatedSize;
	++stackAllocator->allocationCount;
	return ptr;
}

AllocationResult StackAllocator_Pop(StackAllocator* stackAllocator)
{
	u32 lastIndex = stackAllocator->allocationCount - 1;
	void* lastAllocation = stackAllocator->allocations[lastIndex];
	size_t lastAllocationSize = stackAllocator->allocationsSize[lastIndex];
	AllocationResult result = { lastAllocation, lastAllocationSize };
	--stackAllocator->allocationCount;
	return result;
}

AllocationResult StackAllocator_RollbackToAllocation(StackAllocator* stackAllocator, AllocationResult allocation)
{
	void* requestedAllocationPtr = allocation.p;
	size_t difference = &requestedAllocationPtr - &stackAllocator->basePointer;
	logInfo("RequestedAllocPtr: 0x%p. Base pointer: 0x%p. Difference: %zu\n", requestedAllocationPtr, stackAllocator->basePointer, difference);
	return (AllocationResult) { 0 };
}