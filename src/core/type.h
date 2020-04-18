#pragma once

/*** GAME CONTROLS ***/

#define LOGS 1
#if NDEBUG
#define GAME_RELEASE
#else
#define GAME_DEBUG
#endif
#define FAST_AND_ERROR_PRONE 0

/*** TYPES ***/

#include <stdint.h>
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef float f32;
typedef double f64;
#define COUNT_OF(___arr) ((sizeof(___arr)) / (sizeof(___arr[0])))
#define BYTES(n) 	 ((u64)n)
#define KILOBYTES(n) (1024 * BYTES(n))
#define MEGABYTES(n) (1024 * KILOBYTES(n))
#define GIGABYTES(n) (1024 * MEGABYTES(n))
#define TERABYTES(n) (1024 * GIGABYTES(n))
#define VECTOR_TYPE(length) __attribute__((ext_vector_type(length)))
#define VECTOR_TYPE_ALIGNMENT(length, alignment) __attribute__((ext_vector_type(length))) __attribute__((aligned(alignment)))
#define INTRINSIC_INLINE __inline__
#define ALIGNED(bytes) __declspec(align(bytes))

#define my_offsetof(t, d)  __builtin_offsetof(t, d)
