#include "logger.h"
#if LOGS
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#define SHOW_TIME 0
#define SHOW_WRITTEN_BYTES_TO_BUFFER 0
#define PRINT_BUFFER_SIZE MEGABYTES(1)

static struct
{
	char printBuffer[PRINT_BUFFER_SIZE];
	LOG_LEVEL level;
} i_loggerConfig;

static char* i_printBufferPtr = i_loggerConfig.printBuffer;

static inline u64 dontFormat(const char* str)
{
	u64 len = strlen(str);
	for (u64 i = 0; i < len; i++)
	{
		char c = str[i];
		if (c == '%')
			return 0;
	}

	return len;
}

char** getPointerToPrintBuffer(void)
{
	return &i_printBufferPtr;
}

const char* getPrintBuffer(void)
{
	return i_loggerConfig.printBuffer;
}

void initLogger(LOG_LEVEL logLevel)
{
	i_loggerConfig.level = logLevel;
}

void logger(const char* logContext, u32 charCount, LOG_LEVEL level, const char* file, s32 line, const char* fmt, ...)
{
	if (!level || level < i_loggerConfig.level)
		return;

	// TODO: we should lock here if we want to multithread logging
	
	strcpy(i_printBufferPtr, logContext);
	i_printBufferPtr += charCount;

	u64 len = dontFormat(fmt);
	if (len) {
		strcpy(i_printBufferPtr, fmt);
		i_printBufferPtr += len;
		return;
	}

	va_list args;
	va_start(args, fmt);
	s32 bytesWritten = vsprintf(i_printBufferPtr, fmt, args);
	va_end(args);
	assert(i_printBufferPtr + bytesWritten < i_printBufferPtr + PRINT_BUFFER_SIZE);
	i_printBufferPtr += bytesWritten;
	// TODO: we should lock here if we want to multithread logging
}
#endif
