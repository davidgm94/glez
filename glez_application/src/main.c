#include <stdio.h>
#include <GLEZ/glez.h>

void createApplication(void)
{
	
}

void doFrame(void)
{
	f32 r = unitRand();
	f32 g = unitRand();
	f32 b = unitRand();
	f32 a = 1.0f;
	beginTimeBlock(TIME_FRAME_GPU);
	GLEZ_clearColor(r, g, b, a);
}

void deleteApplication(void)
{
	logInfo("Application finishing...\n");
}
