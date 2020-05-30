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

	while (platformUpdate())
	{
		char bf[] = "A\n";
		printf("Hello\n");
		fwrite(bf, sizeof(bf), 1, stdout);
		logInfo("Hello\n");
		float r = (f32)rand() / (f32)RAND_MAX;
		float g = (f32)rand() / (f32)RAND_MAX;
		float b = (f32)rand() / (f32)RAND_MAX;
		glClearColor(r, g, b, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		platformSwapBuffers();
		consumePrintBuffer(0);
	}
}
