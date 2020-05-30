#pragma once
#include "application.h"
extern void doFrame(void);
extern void createApplication(void);
extern void deleteApplication(void);

s32 main(s32 argc, char* argv[])
{
	platformInitialize();
	createApplication();

	bool running = true;
	do
	{
		beginTimeBlock(TIME_FRAME_TOTAL);
		doFrame();
		endFrame();
	} while (running);

	deleteApplication();
}
