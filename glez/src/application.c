#include "application.h"
#include <stdio.h>
#include "logger.h"
#include "frame.h"
#if _WIN32
#include "win32.h"
#else
#pragma error
#endif

void run(void)
{
	logInfo("Engine has started");

	int i = 0;
	while (true)
	{
		platformUpdate(g_Window.windowHandle);
		//consumePrintBuffer(LOG_STDOUT);
	}
}
