//#include "win32.c"
#include "logger.h"
#include "frame.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lightwindows.h"


FrameRecord i_currentFrame;
FrameRecord i_pastFrame;
extern s64 g_TimeFactor;

static inline f32 computeMS(s64 start, s64 end)
{
	return (f32)(end - start) * g_TimeFactor;
}

void consumePrintBuffer(LOG_OUPUT_TYPE type)
{
	f32 currentFrameMiliseconds[TIME_FRAME_ELEMENT_COUNT];
	// TODO: VECTORIZE
	for (s32 i = 0; i < TIME_FRAME_ELEMENT_COUNT; i++)
	{
		f32 start = i_currentFrame.record[i].start;
		f32 end = i_currentFrame.record[i].end;
		currentFrameMiliseconds[i] = computeMS(start, end);
	}

	currentFrameMiliseconds[TIME_FRAME_CPU] = currentFrameMiliseconds[TIME_FRAME_TOTAL] - currentFrameMiliseconds[TIME_FRAME_GPU];
	logInfo("NEW FRAME:\n");

	for (s32 i = 0; i < TIME_FRAME_ELEMENT_COUNT; i++)
	{
		logInfo("\t* [%s] %.02f ms.\n", TIME_BLOCK_STRING[i], currentFrameMiliseconds[i]);
	}
	logInfo("\t* [TIME_FRAME_SUMMARY] CPU: %.02f%%. GPU: %.02f%%\n",
                 (currentFrameMiliseconds[TIME_FRAME_CPU] / currentFrameMiliseconds[TIME_FRAME_TOTAL]) * 100.0f,
                 (currentFrameMiliseconds[TIME_FRAME_GPU] / currentFrameMiliseconds[TIME_FRAME_TOTAL]) * 100.0f);
	
	char** pBuffer = getPointerToPrintBuffer();
	const char* bufferPtr = *pBuffer;
	const char* printBuffer = getPrintBuffer();
	u32 bytesToWrite = bufferPtr - printBuffer;
	OutputDebugStringA("Hello\n");
	printf("Length = %d\n", bytesToWrite);
	fflush(stdout);
	switch (type)
	{
		case (LOG_STDOUT):
		{
			u32 bytesWritten = fwrite(printBuffer, bytesToWrite, 1, stdout);
			assert(bytesWritten == bytesToWrite);
			printf("Bytes to write: %u. Bytes written: %u.\n", bytesToWrite, bytesWritten);
		} break;
		default:
			break;
	}

	*pBuffer = (char*)printBuffer;
}
