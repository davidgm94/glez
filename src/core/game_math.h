#pragma once

#include <math.h>
#include "mmx.h"
#include "sse.h"
void print_vec4f(vec4f v)
{
    printf("PRINTING VECTOR\n");
    for (u32 i = 0; i < 4; i++)
    {
        printf("v[%d] = %f\n", v[i]);
    }
}

typedef f32 vec3f VECTOR_TYPE_ALIGNMENT(3, 16);
typedef f32 mat4x4_simd VECTOR_TYPE_ALIGNMENT(16, 64);
typedef union {
    struct {
        vec4f r0, r1, r2, r3;
    };
    vec4f row[4];
    f32 e[4][4];
    f32 E[16];
    mat4x4_simd e_simd;
} mat4f;

mat4f mat4_get_identity(void)
{
    mat4f mat4 = {
            .r0 = {1.0f, 0.0f, 0.0f, 0.0f},
            .r1 = {0.0f, 1.0f, 0.0f, 0.0f},
            .r2 = {0.0f, 0.0f, 1.0f, 0.0f},
            .r3 = {0.0f, 0.0f, 0.0f, 1.0f},
    };
    return mat4;
}

mat4f mat4f_translate_vec3(mat4f A, vec3f t)
{
    mat4f result = A;
    result.r0[3] += t.x;
    result.r1[3] += t.y;
    result.r2[3] += t.z;

    return result;
}

mat4f mat4f_zrotation(mat4f A, f32 angle)
{
    f32 cos = cosf(angle);
    f32 sin = sinf(angle);

    mat4f r = {
            .row[0] = {cos, -sin, 0.0f, 0.0f},
            .row[1] = {sin, cos, 0.0f, 0.0f},
            .row[2] = {0.0f, 0.0f, 1.0f, 0.0f},
            .row[3] = {0.0f, 0.0f, 0.0f, 1.0f},
    };
}
