#include "buddy_allocator.h"

static const s32 MAX_LEVELS = 32;
void* _freeLists[MAX_LEVELS];


