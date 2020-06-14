#include "string_tools.h"


#define HASHED_STRING_OFFSET 2166136261u 
#define HASHED_STRING_PRIME 16777619u

static inline CalculateFNV(const char* str, size_t offset, size_t prime)
{
	u32 hash = offset;
	while (*str != 0)
	{
		hash ^= *str++;
		hash *= prime;
	}

	return hash;
}

HashedString hashString(const char* s)
{
	size_t hash = CalculateFNV(s, HASHED_STRING_OFFSET, HASHED_STRING_PRIME);
	HashedString h;
	h.hash = hash;
	return h;
}