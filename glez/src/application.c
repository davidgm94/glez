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

void run(void)
{
	logInfo("Engine has started");

	bool running = true;
	while (running)
	{
		BEGIN_TIME_BLOCK(TIME_FRAME_TOTAL);
		float r = (f32)rand() / (f32)RAND_MAX;
		float g = (f32)rand() / (f32)RAND_MAX;
		float b = (f32)rand() / (f32)RAND_MAX;
		
		BEGIN_TIME_BLOCK(TIME_FRAME_GPU);
		glClearColor(r, g, b, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		platformSwapBuffers();
		END_TIME_BLOCK(TIME_FRAME_GPU);
		running = platformUpdate();
		END_TIME_BLOCK(TIME_FRAME_TOTAL);
		consumePrintBuffer(LOG_STDOUT);
	}
}
