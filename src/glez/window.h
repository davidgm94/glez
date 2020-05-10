#pragma once

#include <glez/type.h>
typedef struct {
	s32 width, height;
} w_dimension;

inline w_dimension get_window_dimension(void)
{
	w_dimension wd;
	wd.width = 1024;
	wd.height = 576;
	return wd;
}

w_dimension window_dimension = { 1024, 576 };
