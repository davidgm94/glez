#pragma once

typedef struct IAllocator IAllocator;
typedef struct StackAllocator StackAllocator;
typedef struct AllocationResult AllocationResult;
void StackAllocator_Init(StackAllocator* stackAllocator, size_t requestedSize, IAllocator* backingAllocator);

void* StackAllocator_Allocate(StackAllocator* stackAllocator, size_t size, size_t align);

AllocationResult StackAllocator_Pop(StackAllocator* stackAllocator);

AllocationResult StackAllocator_RollbackToAllocation(StackAllocator* stackAllocator, AllocationResult allocation);

#define STACK_ALLOCATOR_MAX_ALLOCS 1024
typedef struct StackAllocator
{
	void* allocations[STACK_ALLOCATOR_MAX_ALLOCS];
	size_t allocationsSize[STACK_ALLOCATOR_MAX_ALLOCS];
	size_t allocationCount;
	void* stackPointer;
	IAllocator* _backingAllocator;
	size_t totalSize;
	void* basePointer;
} StackAllocator;
