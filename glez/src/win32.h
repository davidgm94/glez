#pragma once
#include "type.h"
#include "lightwindows.h"
s64 g_performanceFrequency;
s64 g_startPerformanceCounter;

#include "win32_internal.h"
#include "openGL/glad.h"
#include <assert.h>
#include "page_allocator.h"

extern HWND g_WindowHandle;
extern f32 g_TimeFactor;
extern HDC g_DC;
extern bool g_Running;

void platformInitialize(void)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&g_performanceFrequency);
	const f32 secondsToMilisecondsFactor = 1000.0f;
	g_TimeFactor = secondsToMilisecondsFactor / (f32)g_performanceFrequency;
	QueryPerformanceCounter((LARGE_INTEGER*)&g_startPerformanceCounter);

	initLogger(1);
	AllocationResult r = reserveVirtualMemory(1, 1, false);
	commitMemory(&r);


	g_WindowHandle = win32_createWindow(GetModuleHandle(NULL), win32_windowProcedure, 1280, 720, "GLEZ Engine", NULL);
	
	s32 gladInit = gladLoadGL();
	assert(gladInit);
	const unsigned char* glVersion = glGetString(GL_VERSION);
	logInfo("OPENGL VERSION: %s\n", glVersion);
#define TESTING 1
#if TESTING
	consumePrintBuffer(LOG_STDOUT);
	ExitProcess(0);
#endif
}

APPLICATION_STATUS platformUpdate(void)
{
	HWND window = g_WindowHandle;
	MSG msg;
	while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	// TODO: Handle other states
	return g_Running ? APPLICATION_RUNNING : APPLICATION_FINISHED;
}

void platformSwapBuffers(void)
{
	// TODO: is this the same for DirectX and Vulkan?
	SwapBuffers(g_DC);
}
