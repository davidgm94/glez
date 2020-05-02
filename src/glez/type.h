#pragma once

/*** GAME CONTROLS ***/

#define LOGS 1
#if NDEBUG
#define GAME_RELEASE
#else
#define GAME_DEBUG
#endif

/*** TYPES ***/

typedef _Bool bool;
#define true 1
#define false 0
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

typedef f32 ALIGNED(16) vec4f VECTOR_TYPE(4);
typedef vec4f quat;
typedef union
{
    vec4f row[4];
    struct
    {
        vec4f r0, r1, r2, r3;
    };
} mat4f;
typedef vec4f vec3f;
#define VEC3(x, y, z) (vec3f) { x, y, z, 0.0f }
#define PURE_QUAT(x, y, z) VEC3(x, y, z)
typedef f32 vec3f_shader[3];
