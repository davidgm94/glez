#pragma once
#include "application.h"
extern void createApplication(void);
extern void deleteApplication(void);

s32 main(s32 argc, char* argv[])
{
	createApplication();
	run();
	deleteApplication();
}
