#pragma once
#include "type.h"
typedef enum LOG_LEVEL {
	LOG_LEVEL_QUIET = 0x00,
	LOG_LEVEL_DEBUG = 0x01,
	LOG_LEVEL_INFO = 0x02,
	LOG_LEVEL_WARN = 0x03,
	LOG_LEVEL_ERROR = 0x04,
	LOG_LEVEL_FATAL = 0x06,
} LOG_LEVEL;

#define LOGS 1
#if LOGS
#define logDebug(...) logger(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define logInfo(...)  logger(LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define logWarn(...)  logger(LOG_LEVEL_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define logError(...) logger(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define logFatal(...) logger(LOG_LEVEL_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void logger(s32 level, const char* file, s32 line, const char* fmt, ...);
char** getPointerToPrintBuffer(void);
const char* getPrintBuffer(void);
#else
#define logDebug(...) 
#define logInfo(...)  
#define logWarn(...)  
#define logError(...) 
#define logFatal(...) 
#endif

