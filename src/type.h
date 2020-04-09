#pragma once

/*** GAME CONTROLS ***/

#if NDEBUG
#define GAME_RELEASE
#define LOGS 0
#else
#define GAME_DEBUG
#define LOGS 1
#endif
#define FAST_AND_ERROR_PRONE 0

/*** TYPES ***/

#include <stdint.h>
typedef uint8_t byte;
typedef uint32_t u32;
typedef uint64_t u64;
#define COUNT_OF(___arr) ((sizeof(___arr)) / (sizeof(___arr[0])))
#define BYTES(n) 	 ((u64)n)
#define KILOBYTES(n) (1024 * BYTES(n))
#define MEGABYTES(n) (1024 * KILOBYTES(n))
#define GIGABYTES(n) (1024 * MEGABYTES(n))
#define TERABYTES(n) (1024 * GIGABYTES(n))
