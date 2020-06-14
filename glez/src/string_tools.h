#pragma once
#include "type.h"

typedef struct HashedString
{
	size_t hash;
} HashedString;

HashedString hashString(const char* s);
