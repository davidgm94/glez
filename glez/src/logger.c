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

#include <stdlib.h>
void logger(const char* logContext, u32 charCount, LOG_LEVEL level, const char* file, s32 fileCharCount, s32 line, const char* fmt, ...)
{
	if (!level || level < i_loggerConfig.level)
		return;

	// TODO: we should lock here if we want to multithread logging
	
	strcpy(i_printBufferPtr, logContext);
	i_printBufferPtr += charCount;
	*i_printBufferPtr++ = '[';
	strcpy(i_printBufferPtr, file);
	i_printBufferPtr += fileCharCount - 1;
	char lineBuffer[16];
	strcpy(i_printBufferPtr, _itoa(line, lineBuffer, 10));
	*i_printBufferPtr = ':';
	i_printBufferPtr += strlen(lineBuffer);
	*i_printBufferPtr++ = ']';
	*i_printBufferPtr++ = '\t';

	va_list args;
	va_start(args, fmt);
	s32 bytesWritten = vsprintf(i_printBufferPtr, fmt, args);
	va_end(args);
	assert(i_printBufferPtr + bytesWritten < i_printBufferPtr + PRINT_BUFFER_SIZE);
	i_printBufferPtr += bytesWritten;
	// TODO: we should lock here if we want to multithread logging
}
#endif
