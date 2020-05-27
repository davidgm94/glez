#pragma once
#include "type.h"
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>

s64 g_performanceFrecuency;
f32 g_factorForMiliseconds;
s64 g_startPerformanceCounter;

inline void initialize(void)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&g_performanceFrecuency);
	g_factorForMiliseconds = g_performanceFrecuency * 1000.0f;
}

static inline f32 getTimeMS(void)
{
	s64 performanceCounter;
	QueryPerformanceCounter((LARGE_INTEGER*)&performanceCounter);
	s64 differencePerformanceCounter = performanceCounter - g_startPerformanceCounter;
	f64 ms = (f64)(differencePerformanceCounter * 1000) / (f64)g_performanceFrecuency;
	return (f32)ms;
}
