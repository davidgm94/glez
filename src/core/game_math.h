#pragma once

//#include <math.h>
//#include <float.h>
//#include <assert.h>
//#include "mmx.h"
//#include "sse.h"
//#include <immintrin.h>
#include <cglm/cglm.h>

//void print_vec4f(vec4f v)
//{
//    printf("PRINTING VECTOR\n");
//    for (u32 i = 0; i < 4; i++)
//    {
//        printf("v[%d] = %f\n", i, v[i]);
//    }
//}
//
//typedef vec4f vec3f;
//typedef f32 mat4x4_simd VECTOR_TYPE_ALIGNMENT(16, 64);
//typedef union {
//    struct {
//        vec4f r0, r1, r2, r3;
//    };
//    vec4f row[4];
//    f32 e[4][4];
//    f32 E[16];
//    mat4x4_simd e_simd;
//} mat4f;
//
//vec4f dot_product_4f(vec4f ax, vec4f ay, vec4f az, vec4f aw, vec4f bx, vec4f by, vec4f bz, vec4f bw)
//{
//    vec4f dx = sse_mul_ps(ax, bx);
//    vec4f dy = sse_mul_ps(ay, by);
//    vec4f dz = sse_mul_ps(az, bz);
//    vec4f dw = sse_mul_ps(aw, bw);
//
//    vec4f a0 = sse_add_ps(dx, dy);
//    vec4f a1 = sse_add_ps(dz, dw);
//    vec4f dots = sse_add_ps(a0, a1);
//
//    return dots;
//}
//
//mat4f mat4_get_identity(void)
//{
//    mat4f mat4 = {
//            .r0 = {1.0f, 0.0f, 0.0f, 0.0f},
//            .r1 = {0.0f, 1.0f, 0.0f, 0.0f},
//            .r2 = {0.0f, 0.0f, 1.0f, 0.0f},
//            .r3 = {0.0f, 0.0f, 0.0f, 1.0f},
//    };
//    return mat4;
//}
//
//mat4f mat4f_translate_vec3(mat4f A, vec3f t)
//{
//    mat4f result = A;
//    result.r0[3] += t.x;
//    result.r1[3] += t.y;
//    result.r2[3] += t.z;
//
//    return result;
//}
//
//vec4f transform4f(mat4f A, vec4f v)
//{
//    vec4f r = {
//            v[0] * A.row[0][0] + v[1] * A.row[0][1] + v[2] * A.row[0][2] + v[3] * A.row[0][3],
//            v[1] * A.row[1][0] + v[1] * A.row[1][1] + v[2] * A.row[1][2] + v[3] * A.row[1][3],
//            v[2] * A.row[2][0] + v[1] * A.row[2][1] + v[2] * A.row[2][2] + v[3] * A.row[2][3],
//            v[3] * A.row[3][0] + v[1] * A.row[3][1] + v[2] * A.row[3][2] + v[3] * A.row[3][3],
//    };
//
//    return r;
//}
//
//vec3f transform3f(mat4f A, vec3f v)
//{
//    vec3f r = transform4f(A, v);
//    return r;
//}
//
//mat4f xrotation(f32 angle)
//{
//    f32 cos = cosf(angle);
//    f32 sin = sinf(angle);
//
//    mat4f r = {
//            .r0 = { 1.0f, 0.0f, 0.0f, 0.0f },
//            .r1 = { 0.0f, cos, -sin, 0.0f },
//            .r2 = { 0.0f, sin, cos, 0.0f },
//            .r3 = { 0.0f, 0.0f, 0.0f, 1.0f },
//    };
//
//    return r;
//}
//
//mat4f yrotation(f32 angle)
//{
//    f32 cos = cosf(angle);
//    f32 sin = sinf(angle);
//
//    mat4f r = {
//            .r0 = { cos, 0.0f, sin, 0.0f },
//            .r1 = { 0.0f, 1.0f, 0.0f, 0.0f },
//            .r2 = { -sin, 0.0f, cos, 0.0f },
//            .r3 = { 0.0f, 0.0f, 0.0f, 1.0f },
//    };
//
//    return r;
//}
//
//mat4f zrotation(f32 angle)
//{
//    f32 cos = cosf(angle);
//    f32 sin = sinf(angle);
//
//    mat4f r = {
//            .row[0] = {cos, -sin, 0.0f, 0.0f},
//            .row[1] = {sin, cos, 0.0f, 0.0f},
//            .row[2] = {0.0f, 0.0f, 1.0f, 0.0f},
//            .row[3] = {0.0f, 0.0f, 0.0f, 1.0f},
//    };
//
//    return r;
//}
//
//mat4f translate(mat4f m, vec3f v)
//{
//    m.row[3] = m.r0 * v[0] + m.r1 * v[1] + m.r2 * v[2] + m.r3;
//    return m;
//}
//
//mat4f translation(vec3f t)
//{
//    mat4f r = {
//            .row[0] = { 1.0f, 0.0f, 0.0f, t[0] },
//            .row[1] = { 0.0f, 1.0f, 0.0f, t[1] },
//            .row[2] = { 0.0f, 0.0f, 1.0f, t[2] },
//            .row[3] = { 0.0f, 0.0f, 0.0f, t[3] },
//    };
//
//    return r;
//}
//
//mat4f transpose(mat4f A)
//{
//    mat4f r;
//    for (int i = 0; i < 4; i++)
//        for (int j = 0; j < 4; j++)
//            r.row[i][j] = A.row[j][i];
//    return r;
//}
//
//f32 radians(f32 degrees)
//{
//    f64 d_degrees = (f64) degrees;
//    f64 d_factor = 0.01745329251994329576923690768489;
//    return (f32)(d_degrees * d_factor);
//}
//mat4f perspective(f32 fovy, f32 aspect, f32 z_near, f32 z_far)
//{
//    assert(fabsf(aspect - FLT_EPSILON) > 0.0f);
//    const f32 tan_half_fovy = tanf(fovy / 2.0f);
//
//    mat4f m = {(float)0};
//    m.row[0][0] = 1.0f / (aspect * tan_half_fovy);
//    m.row[1][1] = 1.0f / tan_half_fovy;
//    m.row[2][2] = - (z_far + z_near) / (z_far - z_near);
//    m.row[2][3] = -1.0f;
//    m.row[3][2] = - (2.0f * z_far * z_near) / (z_far - z_near);
//
//    return m;
//}
//
//mat4f rotate(mat4f m, mat4f rotation_m)
//{
//    mat4f r = {
//            .r0 = { m.row[0] * rotation_m.row[0][0] + m.row[1] * rotation_m.row[0][1] + m.row[2] * rotation_m.row[0][2] },
//            .r1 = { m.row[0] * rotation_m.row[1][0] + m.row[1] * rotation_m.row[1][1] + m.row[2] * rotation_m.row[1][2] },
//            .r2 = { m.row[0] * rotation_m.row[2][0] + m.row[1] * rotation_m.row[2][1] + m.row[2] * rotation_m.row[2][2] },
//            .r3 = m.row[3],
//    };
//
//    return r;
//}
//
////inline f32 length_3f(vec3f v)
////{
////    return sse_cvtss_f32(sse_sqrt_ps(_mm_dp_ps(v, v, 0x71)));
////}
////
////vec3f normalize_3f(vec3f v)
////{
////    vec3f inverse_norm = sse_rsqrt_ps(_mm_dp_ps(v, v, 0x77));
////    return sse_mul_ps(v, inverse_norm);
////}
//f32 inner(vec3f a, vec3f b)
//{
//    return a.x * b.x + a.y * b.y + a.z * b.z;
//}
//
//f32 length_sq_3f(vec3f v)
//{
//    f32 result = inner(v, v);
//    return result;
//}
//f32 length_3f(vec3f v)
//{
//    f32 result = sqrtf(length_sq_3f(v));
//    return result;
//}
//
//vec3f normalize_3f(vec3f v)
//{
//    vec3f result = v * (1.0f / length_3f(v));
//    return result;
//}
//
//mat4f rotate_complex(mat4f m, f32 angle, vec3f v)
//{
//    f32 cos = cosf(angle);
//    f32 sin = sinf(angle);
//
//    vec3f axis = normalize_3f(v);
//    vec3f temp = (1.0f - cos) * axis;
//
//    mat4f rotate;
//    rotate.r0[0] = cos + temp[0] * axis[0];
//    rotate.r0[1] = temp[0] * axis[1] + sin * axis[2];
//    rotate.r0[2] = temp[0] * axis[2] - sin * axis[1];
//
//    rotate.r1[0] = temp[1] * axis[0] - sin * axis[2];
//    rotate.r1[1] = cos + temp[1] * axis[1];
//    rotate.r1[2] = temp[1] * axis[2] + sin * axis[0];
//
//    rotate.r2[0] = temp[2] * axis[0] + sin * axis[1];
//    rotate.r2[1] = temp[2] * axis[1] - sin * axis[0];
//    rotate.r2[2] = cos + temp[2] * axis[2];
//
//    mat4f result = {
//            .r0 = m.r0 * rotate.r0[0] + m.r1 * rotate.r0[1] + m.r2 * rotate.r0[2],
//            .r1 = m.r0 * rotate.r1[0] + m.r1 * rotate.r1[1] + m.r2 * rotate.r1[2],
//            .r2 = m.r0 * rotate.r2[0] + m.r1 * rotate.r2[1] + m.r2 * rotate.r2[2],
//            .r3 = m.r3,
//    };
//
//    return result;
//}