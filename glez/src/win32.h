#pragma once
#include "type.h"
#include "lightwindows.h"
s64 g_performanceFrequency;
s64 g_startPerformanceCounter;

#include "win32_internal.h"

extern HWND g_WindowHandle;
extern f32 g_TimeFactor;
extern HDC g_DC;

void platformInitialize(void)
{
	initLogger(1);
	g_WindowHandle = win32_createWindow(GetModuleHandle(NULL), win32_windowProcedure, 1024, 576, "WindowTitle", NULL);
	
	QueryPerformanceFrequency((LARGE_INTEGER*)&g_performanceFrequency);
	g_TimeFactor = 1000.0f / (f32)g_performanceFrequency;
}

APPLICATION_STATUS platformUpdate(void)
{
	HWND window = g_WindowHandle;
	MSG msg = {0};
	while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) {
			return APPLICATION_FINISHED;
		}
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
	return APPLICATION_RUNNING;
}

void platformSwapBuffers(void)
{
	// TODO: is this the same for DirectX and Vulkan?
	SwapBuffers(g_DC);
}
