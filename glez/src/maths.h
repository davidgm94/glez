#pragma once

#include <math.h>
#include <stdlib.h>
#define abs_T(x) (((x) > 0) ? (x) : -(x))

static inline f32 unitRand(void)
{
	f32 result = (f32)rand() / (f32)RAND_MAX;
	return result;
}
