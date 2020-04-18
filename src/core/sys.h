#pragma once
#define LOG_ERROR_AND_EXIT(message) fwrite(message, COUNT_OF(message), 1, stdout); _exit(-1);

