#include "application.h"
#include <stdio.h>
#include "logger.h"
#include "frame.h"
#if _WIN32
#include "win32.h"
#else
#pragma error
#endif
#include <stdlib.h>

bool applicationRuns(void)
{
	// First frame stats are wrong
	platformSwapBuffers();
	endTimeBlock(TIME_FRAME_GPU);
	bool running = platformUpdate();
	endTimeBlock(TIME_FRAME_TOTAL);
	consumePrintBuffer(LOG_STDOUT);
	beginTimeBlock(TIME_FRAME_TOTAL);
	return running;
}

void beginTimeBlock(TIME_BLOCK timeblock)
{
	QueryPerformanceCounter((LARGE_INTEGER*)&i_CurrentFrame.record[timeblock].start);
}
void endTimeBlock(TIME_BLOCK timeblock)
{
	QueryPerformanceCounter((LARGE_INTEGER*)&i_CurrentFrame.record[timeblock].end);
}

GLEZ_API void destroyEngine(void)
{
	DestroyWindow(g_WindowHandle);
	consumePrintBuffer(LOG_STDOUT);
	ExitProcess(0);
}
