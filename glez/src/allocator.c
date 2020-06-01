#include "allocator.h"

void allocate(void)
{
	struct
	{
		int b;
		char c;
	};
	int a = __alignof(size_t);
	b = a;
	c = 'a';
	return;
}
