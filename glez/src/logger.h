#pragma once
#include "type.h"

#define log_debug(...)
#define log_info(...)
#define log_warn(...)
#define log_error(...)
#define log_fatal(...)

static struct
{
	s32 level;

};
void logger();
