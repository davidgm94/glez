#pragma once
#include "application.h"
extern void doFrame(void);
extern void createApplication(void);
extern void deleteApplication(void);

#include <stdio.h>
s32 main(s32 argc, char* argv[])
{
	platformInitialize();
	createApplication();

	do
	{
		// GPU-timing should be set in user code
		doFrame();
	} while (applicationRuns());

	deleteApplication();
	destroyEngine();
}
