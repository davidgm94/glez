#pragma once
#include "type.h"


ENUM LOG_OUPUT_TYPE{
	LOG_STDOUT,
	LOG_OUTPUT_DEBUG_STRING,
	LOG_FILE,
} LOG_OUPUT_TYPE;

void consumePrintBuffer(LOG_OUPUT_TYPE type);

