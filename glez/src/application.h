#pragma once
#include "type.h"

typedef enum APPLICATION_STATUS
{
	APPLICATION_FINISHED = 0x00,
	APPLICATION_RUNNING = 0x01,
	APPLICATION_PAUSED = 0x02,
} APPLICATION_STATUS;

GLEZ_API void platformInitialize(void);
APPLICATION_STATUS platformUpdate(void);
GLEZ_API bool endFrame(void);
GLEZ_API void beginTimeBlock(TIME_BLOCK timeblock);
GLEZ_API void endTimeBlock(TIME_BLOCK timeblock);
