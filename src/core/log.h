#pragma once

#if _WIN32
#elif __linux__
#include <bits/types/struct_timespec.h>
#endif
#include <time.h>
#include "type.h"

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
#define FOREACH_TIME_BLOCK(TIME_BLOCK)\
    TIME_BLOCK(TIME_FRAME_TOTAL)\
    TIME_BLOCK(TIME_FRAME_CPU)\
    TIME_BLOCK(TIME_FRAME_GPU)\
    /*TIME_BLOCK(TIME_FRAME_UPDATE)*/\
    TIME_BLOCK(TIME_FRAME_ELEMENT_COUNT)

typedef enum TIME_BLOCK
{
    FOREACH_TIME_BLOCK(GENERATE_ENUM)
} TIME_BLOCK;

static const char* TIME_BLOCK_STRING[] =
{
    FOREACH_TIME_BLOCK(GENERATE_STRING)
};

typedef struct timing_record {
    struct timespec start;
    struct timespec end;
    f32 ms;
} timing_record;

typedef struct frametime_record {
    struct timing_record record[TIME_FRAME_ELEMENT_COUNT];
} frametime_record;

extern void frame_logger(char* fmt, ...);
extern void frame_log_and_clear(void);

extern frametime_record current_frame;

//#define BEGIN_TIME_BLOCK(name) clock_gettime(CLOCK_MONOTONIC, &current_frame.record[name].start)
//#define END_TIME_BLOCK(name) clock_gettime(CLOCK_MONOTONIC, &current_frame.record[name].end)
