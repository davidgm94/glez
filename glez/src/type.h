#pragma once

/*** GAME CONTROLS ***/

#define LOGS 1
#if NDEBUG
#define GLEZ_RELEASE 1
#else
#define GLEZ_DEBUG 1
#endif

#ifdef GLEZ_PLATFORM_WINDOWS
	#ifdef GLEZ_BUILD_DLL
		#define GLEZ_API __declspec(dllexport)
	#else
		#define GLEZ_API __declspec(dllimport)
	#endif
#else
#pragma error "Windows is the only platform supported at the moment"
#endif

/*** TYPES ***/

#define STRUCT typedef struct
#define ENUM typedef enum
#define UNION typedef union
#ifndef __cplusplus
typedef _Bool bool;
#define true 1
#define false 0
#endif
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
UNION
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
#define WIN32_CHECK(_hr) do { s32 hr = (s32)(_hr); if (hr < 0) { printf("D3D FAIL! CODE: %d\n", hr); assert(0); } } while(0)
#define UUID_MACRO(_type, _obj) (__uuidof(_type)), ((void**)(&_obj))

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

#define BEGIN_TIME_BLOCK(name) QueryPerformanceCounter((LARGE_INTEGER*)&i_CurrentFrame.record[name].start)
#define END_TIME_BLOCK(name) QueryPerformanceCounter((LARGE_INTEGER*)&i_CurrentFrame.record[name].end)
#define _FOREACH_TIME_BLOCK(TIME_BLOCK)\
    TIME_BLOCK(TIME_FRAME_TOTAL)\
    TIME_BLOCK(TIME_FRAME_CPU)\
    TIME_BLOCK(TIME_FRAME_GPU)\
    /*TIME_BLOCK(TIME_FRAME_UPDATE)*/\
	TIME_BLOCK(TIME_FRAME_ELEMENT_COUNT)

ENUM TIME_BLOCK {
_FOREACH_TIME_BLOCK(GENERATE_ENUM)
} TIME_BLOCK;

static const char* TIME_BLOCK_STRING[] =
{
	_FOREACH_TIME_BLOCK(GENERATE_STRING)
};

STRUCT TimingRecord
{
#if GLEZ_PLATFORM_WINDOWS
	s64 start;
	s64 end;
#endif
} TimingRecord;

STRUCT FrameRecord
{
	TimingRecord record[TIME_FRAME_ELEMENT_COUNT];
} FrameRecord;

extern FrameRecord i_CurrentFrame;

