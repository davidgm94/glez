#pragma once
#include "type.h"
struct SlotAllocator;
struct PageAllocator;
struct HeapAllocator;
typedef struct {
	struct SlotAllocator* slotAllocator;
	struct PageAllocator* pageAllocator;
	struct HeapAllocator* heapAllocator;
} Allocators;
