#pragma once
#include "type.h"
#include "lightwindows.h"
s64 g_performanceFrequency;
s64 g_startPerformanceCounter;
#include "win32_internal.h"

void platformInitialize(void)
{
	
	logInfo("Initializing Windows");
	g_Window.windowHandle = win32_createWindow(GetModuleHandle(NULL), win32_windowProcedure, 1024, 576, "WindowTitle", NULL);
	
	QueryPerformanceFrequency((LARGE_INTEGER*)&g_performanceFrequency);
	g_timeFactor = g_performanceFrequency * 1000.0f;
	printf("QueryPerformanceFrequency returned: %llu\n", (unsigned long long)g_performanceFrequency);
}

void platformUpdate(void* windowHandle)
{
	HWND window = (HWND)windowHandle;
	MSG msg = {0};
	while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

f32 win32_getTimeMS(void)
{
	s64 performanceCounter;
	QueryPerformanceCounter((LARGE_INTEGER*)&performanceCounter);
	s64 differencePerformanceCounter = performanceCounter - g_startPerformanceCounter;
	f64 ms = (f64)(differencePerformanceCounter * 1000) / (f64)g_performanceFrequency;
	return (f32)ms;
}
