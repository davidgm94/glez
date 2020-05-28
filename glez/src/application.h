#pragma once
#include "core.h"
#include "type.h"
s64 g_timeFactor;
STRUCT Window
{
	void* windowHandle;
	s32 width, height;
} Window;

Window g_Window;

GLEZ_API void platformInitialize(void);
void platformUpdate(void* windowHandle);
GLEZ_API void run(void);
