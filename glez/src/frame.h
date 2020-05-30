#pragma once
#include "type.h"

#define FOREACH_TIME_BLOCK(TIME_BLOCK)\
    TIME_BLOCK(TIME_FRAME_TOTAL)\
    TIME_BLOCK(TIME_FRAME_CPU)\
    TIME_BLOCK(TIME_FRAME_GPU)\
    /*TIME_BLOCK(TIME_FRAME_UPDATE)*/\
	TIME_BLOCK(TIME_FRAME_ELEMENT_COUNT)

ENUM TIME_BLOCK {
FOREACH_TIME_BLOCK(GENERATE_ENUM)
} TIME_BLOCK;

static const char* TIME_BLOCK_STRING[] =
{
	FOREACH_TIME_BLOCK(GENERATE_STRING)
};

STRUCT TimingRecord
{
#if GLEZ_PLATFORM_WINDOWS
	s64 start;
	s64 end;
#endif
} TimingRecord;

STRUCT FrameRecord
{
	TimingRecord record[TIME_FRAME_ELEMENT_COUNT];
} FrameRecord;

ENUM LOG_OUPUT_TYPE{
	LOG_STDOUT,
	LOG_OUTPUT_DEBUG_STRING,
	LOG_FILE,
} LOG_OUPUT_TYPE;

void consumePrintBuffer(LOG_OUPUT_TYPE type);

extern FrameRecord i_CurrentFrame;
#define BEGIN_TIME_BLOCK(name) QueryPerformanceCounter((LARGE_INTEGER*)&i_CurrentFrame.record[name].start)
#define END_TIME_BLOCK(name) QueryPerformanceCounter((LARGE_INTEGER*)&i_CurrentFrame.record[name].end)
