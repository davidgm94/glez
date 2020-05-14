#pragma once

#include <glez/type.h>
#include <math.h>
#include <assert.h>
#include <immintrin.h>
#define SIMD 1

#define PI_F32 3.14159265358979323846264338327950288

#define MAT4_ZERO_INIT          {.r0 = (vec4f){0.0f, 0.0f, 0.0f, 0.0f},                    \
                                 .r1 = (vec4f){0.0f, 0.0f, 0.0f, 0.0f},                    \
                                 .r2 = (vec4f){0.0f, 0.0f, 0.0f, 0.0f},                    \
                                 .r3 = (vec4f){0.0f, 0.0f, 0.0f, 0.0f}}

#define MAT4_IDENTITY_INIT      {.r0 = (vec4f){1.0f, 0.0f, 0.0f, 0.0f},                    \
                                 .r1 = (vec4f){0.0f, 1.0f, 0.0f, 0.0f},                    \
                                 .r2 = (vec4f){0.0f, 0.0f, 1.0f, 0.0f},                    \
                                 .r3 = (vec4f){0.0f, 0.0f, 0.0f, 1.0f}}

#define MAT4F_ZERO {\
        .r0 = {0.0f, 0.0f, 0.0f, 0.0f},\
        .r1 = {0.0f, 0.0f, 0.0f, 0.0f},\
        .r2 = {0.0f, 0.0f, 0.0f, 0.0f},\
        .r3 = {0.0f, 0.0f, 0.0f, 0.0f},\
    }
 
/* for C only */
#define MAT4_IDENTITY ((mat4f)MAT4_IDENTITY_INIT)
#define MAT4_ZERO     ((mat4f)MAT4_ZERO_INIT)
#define VEC3_RIGHT VEC3(1.0f, 0.0f, 0.0f)

static inline vec3f vec3_closer(vec3f a, vec3f b)
{
#if SIMD
//#pragma error
    return (vec3f){ 0, 0, 0, 0 };
#else
    f32 a_length_squared = (a.x * a.x) + (a.y * a.y) + (a.z * a.z);
    f32 b_length_squared = (b.x * b.x) + (b.y * b.y) + (b.z * b.z);
    return a_length_squared < b_length_squared ? b : a;
#endif
}

static inline vec3f vec3_muladds(vec3f a, f32 s, vec3f dest)
{
#if SIMD
    vec3f scalar_vector = _mm_set1_ps(s);
    vec3f mult = _mm_mul_ps(a, scalar_vector);
    dest = _mm_add_ps(mult, dest);
#else
	dest[0] += a[0] * s;
	dest[1] += a[1] * s;
	dest[2] += a[2] * s;
#endif
    return dest;
}

static inline bool vec3_equal(vec3f a, vec3f b)
{
#if SIMD
	return (_mm_movemask_ps(_mm_cmpeq_ps(a, b)) & 7) == 7;
#else
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
#endif
}

static inline vec3f vec3_cross(vec3f a, vec3f b)
{
#if SIMD
    /* (u2.v3 - u3.v2, u3.v1 - u1.v3, u1.v2 - u2.v1) */
    vec3f first_mult_first_operand = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 2, 1));
    vec3f first_mult_second_operand = _mm_shuffle_ps(b, b, _MM_SHUFFLE(0, 1, 0, 2));
    vec3f second_mult_first_operand = _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 1, 0, 2));
    vec3f second_mult_second_operand = _mm_shuffle_ps(b, b, _MM_SHUFFLE(0, 0, 2, 1));
    vec3f first_mult = _mm_mul_ps(first_mult_first_operand, first_mult_second_operand);
    vec3f second_mult = _mm_mul_ps(second_mult_first_operand, second_mult_second_operand);
    vec3f dest = _mm_sub_ps(first_mult, second_mult);
    
    return dest;
#else
    vec3f dest;
    dest[0] = a[1] * b[2] - a[2] * b[1];
    dest[1] = a[2] * b[0] - a[0] * b[2];
    dest[2] = a[0] * b[1] - a[1] * b[0];
    return dest;
#endif
}

typedef enum
{
    COMPUTE_1_ELEM = 0x11,
    COMPUTE_2_ELEMS = 0x33,
    COMPUTE_3_ELEMS = 0x77,
    COMPUTE_4_ELEMS = 0xFF,
} VEC4F_INDEX_FLAG;

static inline f32 vec3_dot(vec3f a, vec3f b)
{
#if SIMD
    //int mask = (PICK_VEC4F_ELEM(false, true, true, true) << 4) | PICK_VEC4F_ELEM(false, true, true, true);
    vec3f result = _mm_dp_ps(a, b, COMPUTE_3_ELEMS);
    return result[0];
#else
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
#endif
}

static inline vec3f vec3_dot_vec(vec3f a, vec3f b)
{
#if SIMD
    //int mask = (PICK_VEC4F_ELEM(false, true, true, true) << 4) | PICK_VEC4F_ELEM(false, true, true, true);
    vec3f result = _mm_dp_ps(a, b, COMPUTE_3_ELEMS);
    return result;
#else
    float dp = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    return (vec3f) { dp, dp, dp, 0.0f };
#endif
}

static inline f32 rad(f32 deg)
{
    return deg * PI_F32 / 180.0f;
}
 
static inline f32 vec3_norm(vec3f v)
{
    return _mm_sqrt_ps(_mm_dp_ps(v, v, COMPUTE_3_ELEMS))[0];
}

static inline vec3f vec3_norm_vec(vec3f v)
{
    return _mm_sqrt_ps(_mm_dp_ps(v, v, COMPUTE_3_ELEMS));
}
 
static inline vec3f vec3_scale(vec3f v, f32 s)
{
#if SIMD
    vec3f s_vector = { s, s, s };
    return _mm_mul_ps(v, s_vector);
#else
    vec3f vector = { v[0] * s, v[1] * s, v[2] * s };
    return vector;
#endif
}

static inline vec3f vec3_normalize(vec3f v)
{
#if SIMD
    vec3f norm = vec3_norm_vec(v);
    vec3f inv = { 1.0f, 1.0f, 1.0f };
    vec3f inverse = _mm_div_ps(inv, norm);
    vec3f result = _mm_mul_ps(v, inverse);
    return result;
#else
	f32 invmag = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] *= invmag;
	v[1] *= invmag;
	v[2] *= invmag;
    return v;
#endif
}

static inline vec3f vec3_add(vec3f a, vec3f b)
{
    return _mm_add_ps(a, b);
}

static inline vec3f vec3_sub(vec3f a, vec3f b)
{
    return _mm_sub_ps(a, b);
}

// TODO: parallelize
static inline mat4f perspective(f32 fovy, f32 aspect, f32 nearVal, f32 farVal)
{
    mat4f result = MAT4F_ZERO;
	f32 f, fn;

	f = 1.0f / tanf(fovy * 0.5f);
	fn = 1.0f / (nearVal - farVal);

	result.row[0][0] = f / aspect;
	result.row[1][1] = f;
	result.row[2][2] = (nearVal + farVal) * fn;
	result.row[2][3] = -1.0f;
	result.row[3][2] = 2.0f * nearVal * farVal * fn;

    return result;
}

static inline vec3f vec3_crossn(vec3f a, vec3f b)
{
    vec3f result = vec3_cross(a, b);
	return vec3_normalize(result);
}

static inline mat4f lookat(vec3f eye, vec3f center, vec3f up)
{
    mat4f result;
	vec3f f = vec3_sub(center, eye);
	f = vec3_normalize(f);

	vec3f s = vec3_crossn(f, up);
    vec3f u = vec3_cross(s, f);
	result.row[0][0] = s[0];
	result.row[0][1] = u[0];
	result.row[0][2] = -f[0];
	result.row[1][0] = s[1];
	result.row[1][1] = u[1];
	result.row[1][2] = -f[1];
	result.row[2][0] = s[2];
	result.row[2][1] = u[2];
	result.row[2][2] = -f[2];
	result.row[3][0] = -vec3_dot(s, eye);
	result.row[3][1] = -vec3_dot(u, eye);
	result.row[3][2] = vec3_dot(f, eye);
	result.row[0][3] = result.row[1][3] = result.row[2][3] = 0.0f;
	result.row[3][3] = 1.0f;

    return result;
}

static inline vec4f vec4_scale(vec4f a, f32 s)
{
#if SIMD
    vec4f scalar_vector = { s, s, s, s };
    return _mm_mul_ps(a, scalar_vector);
#else
    vec4f dest;
	dest[0] = a[0] * s;
	dest[1] = a[1] * s;
	dest[2] = a[2] * s;
	dest[3] = a[3] * s;
    return dest;
#endif
}

static inline vec4f vec4_add(vec4f a, vec4f b)
{
#if SIMD
    return _mm_add_ps(a, b);
#else
    vec4f result;
	result[0] = a[0] + b[0];
	result[1] = a[1] + b[1];
	result[2] = a[2] + b[2];
	result[3] = a[3] + b[3];
    return result;
#endif
}

static inline mat4f translate(mat4f m, vec3f v)
{
#if 0
	glmm_store(m[3],
		_mm_add_ps(_mm_add_ps(_mm_mul_ps(glmm_load(m[0]),
			_mm_set1_ps(v[0])),
			_mm_mul_ps(glmm_load(m[1]),
				_mm_set1_ps(v[1]))),
			_mm_add_ps(_mm_mul_ps(glmm_load(m[2]),
				_mm_set1_ps(v[2])),
				glmm_load(m[3]))))
		;
#else
	vec4f v1 = vec4_scale(m.row[0], v[0]);
	vec4f v2 = vec4_scale(m.row[1], v[1]);
	vec4f v3 = vec4_scale(m.row[2], v[2]);

    m.row[3] = vec4_add(v1, m.row[3]);
    m.row[3] = vec4_add(v2, m.row[3]);
    m.row[3] = vec4_add(v3, m.row[3]);

    return m;
#endif
}

static inline vec3f vec3_normalize_to(vec3f v)
{
	f32 norm = vec3_norm(v);
	if (norm == 0.0f)
	{
		
        return (vec3f) { 0.0f, 0.0f, 0.0f };
	}

    vec3f result = vec3_scale(v, 1.0f / norm);
    return result;
}

static inline mat4f rotate_make(f32 angle, vec3f axis)
{
	f32 c = cosf(angle);

    vec3f axisn = vec3_normalize_to(axis);
    vec3f v = vec3_scale(axisn, 1.0f - c);
    vec3f vs = vec3_scale(axisn, sinf(angle));

    mat4f m;
	m.row[0] = vec3_scale(axisn, v[0]);
	m.row[1] = vec3_scale(axisn, v[1]);
	m.row[2] = vec3_scale(axisn, v[2]);

	m.row[0][0] += c;       m.row[1][0] -= vs[2];   m.row[2][0] += vs[1];
	m.row[0][1] += vs[2];   m.row[1][1] += c;       m.row[2][1] -= vs[0];
	m.row[0][2] -= vs[1];   m.row[1][2] += vs[0];   m.row[2][2] += c;

	m.row[0][3] = m.row[1][3] = m.row[2][3] = m.row[3][0] = m.row[3][1] = m.row[3][2] = 0.0f;
	m.row[3][3] = 1.0f;

    return m;
}

static inline mat4f mul_rot(mat4f m1, mat4f m2)
{
#if SIMD
    mat4f result;
   
    f32 m2_00 = m2.row[0][0];
    f32 m2_01 = m2.row[0][1];
    f32 m2_02 = m2.row[0][2];
    vec4f m2_00_v = { m2_00, m2_00, m2_00, m2_00 };
    vec4f m2_01_v = { m2_01, m2_01, m2_01, m2_01 };
    vec4f m2_02_v = { m2_02, m2_02, m2_02, m2_02 };
    vec4f first_row_first_mul = _mm_mul_ps(m1.row[0], m2_00_v);
    vec4f first_row_second_mul = _mm_mul_ps(m1.row[1], m2_01_v);
    vec4f first_row_third_mul = _mm_mul_ps(m1.row[2], m2_02_v);

    result.row[0] = _mm_add_ps(_mm_add_ps(first_row_first_mul, first_row_second_mul), first_row_third_mul);

    f32 m2_10 = m2.row[1][0];
    f32 m2_11 = m2.row[1][1];
    f32 m2_12 = m2.row[1][2];
    vec4f m2_10_v = { m2_10, m2_10, m2_10, m2_10 };
    vec4f m2_11_v = { m2_11, m2_11, m2_11, m2_11 };
    vec4f m2_12_v = { m2_12, m2_12, m2_12, m2_12 };
    vec4f second_row_first_mul = _mm_mul_ps(m1.row[0], m2_10_v);
    vec4f second_row_second_mul = _mm_mul_ps(m1.row[1], m2_11_v);
    vec4f second_row_third_mul = _mm_mul_ps(m1.row[2], m2_12_v);

    result.row[1] = _mm_add_ps(_mm_add_ps(second_row_first_mul, second_row_second_mul), second_row_third_mul);

    f32 m2_20 = m2.row[2][0];
    f32 m2_21 = m2.row[2][1];
    f32 m2_22 = m2.row[2][2];
    vec4f m2_20_v = { m2_20, m2_20, m2_20, m2_20 };
    vec4f m2_21_v = { m2_21, m2_21, m2_21, m2_21 };
    vec4f m2_22_v = { m2_22, m2_22, m2_22, m2_22 };
    vec4f third_row_first_mul = _mm_mul_ps(m1.row[0], m2_20_v);
    vec4f third_row_second_mul = _mm_mul_ps(m1.row[1], m2_21_v);
    vec4f third_row_third_mul = _mm_mul_ps(m1.row[2], m2_22_v);

    result.row[2] = _mm_add_ps(_mm_add_ps(third_row_first_mul, third_row_second_mul), third_row_third_mul);

    result.row[3] = m1.row[3];

    return result;
#else
	f32 a00 = m1.row[0][0], a01 = m1.row[0][1], a02 = m1.row[0][2], a03 = m1.row[0][3],
		a10 = m1.row[1][0], a11 = m1.row[1][1], a12 = m1.row[1][2], a13 = m1.row[1][3],
		a20 = m1.row[2][0], a21 = m1.row[2][1], a22 = m1.row[2][2], a23 = m1.row[2][3],
		a30 = m1.row[3][0], a31 = m1.row[3][1], a32 = m1.row[3][2], a33 = m1.row[3][3],

		b00 = m2.row[0][0], b01 = m2.row[0][1], b02 = m2.row[0][2],
		b10 = m2.row[1][0], b11 = m2.row[1][1], b12 = m2.row[1][2],
		b20 = m2.row[2][0], b21 = m2.row[2][1], b22 = m2.row[2][2];

    mat4f dest;
	dest.row[0][0] = a00 * b00 + a10 * b01 + a20 * b02;
	dest.row[0][1] = a01 * b00 + a11 * b01 + a21 * b02;
	dest.row[0][2] = a02 * b00 + a12 * b01 + a22 * b02;
	dest.row[0][3] = a03 * b00 + a13 * b01 + a23 * b02;

	dest.row[1][0] = a00 * b10 + a10 * b11 + a20 * b12;
	dest.row[1][1] = a01 * b10 + a11 * b11 + a21 * b12;
	dest.row[1][2] = a02 * b10 + a12 * b11 + a22 * b12;
	dest.row[1][3] = a03 * b10 + a13 * b11 + a23 * b12;

	dest.row[2][0] = a00 * b20 + a10 * b21 + a20 * b22;
	dest.row[2][1] = a01 * b20 + a11 * b21 + a21 * b22;
	dest.row[2][2] = a02 * b20 + a12 * b21 + a22 * b22;
	dest.row[2][3] = a03 * b20 + a13 * b21 + a23 * b22;

	dest.row[3][0] = a30;
	dest.row[3][1] = a31;
	dest.row[3][2] = a32;
	dest.row[3][3] = a33;

	return dest;
#endif
}
static inline mat4f scale_to(mat4f m, vec3f v)
{
    mat4f result;
    result.row[0] = vec4_scale(m.row[0], v[0]);
    result.row[1] = vec4_scale(m.row[1], v[1]);
    result.row[2] = vec4_scale(m.row[2], v[2]);
    result.row[3] = m.row[3];

    return result;
}

static inline mat4f scale(mat4f m, vec3f v)
{
    return scale_to(m, v);
}

static inline mat4f rotate(mat4f m, f32 angle, vec3f axis)
{
	mat4f rot = rotate_make(angle, axis);
	m = mul_rot(m, rot);
    return m;
}

static inline vec4f dot_4wide(vec4f ax, vec4f ay, vec4f az, vec4f aw, vec4f bx, vec4f by, vec4f bz, vec4f bw)
{
    vec4f dx = _mm_mul_ps(ax, bx);
    vec4f dy = _mm_mul_ps(ay, by);
    vec4f dz = _mm_mul_ps(az, bz);
    vec4f dw = _mm_mul_ps(aw, bw);

    return _mm_add_ps(_mm_add_ps(dx, dy), _mm_add_ps(dz, dw));
}

// Refactor data
static inline mat4f mat4f_mul(mat4f a, mat4f b)
{
#if SIMD
    mat4f result;
    for (s32 i = 0; i < 4; i++)
    {
		vec4f bx = _mm_set_ps(b.row[0][0], b.row[1][0], b.row[2][0], b.row[3][0]);
		vec4f by = _mm_set_ps(b.row[0][1], b.row[1][1], b.row[2][1], b.row[3][1]);
		vec4f bz = _mm_set_ps(b.row[0][2], b.row[1][2], b.row[2][2], b.row[3][2]);
		vec4f bw = _mm_set_ps(b.row[0][3], b.row[1][3], b.row[2][3], b.row[3][3]);

		vec4f dx = _mm_mul_ps(a.row[i], bx);
		vec4f dy = _mm_mul_ps(a.row[i], by);
		vec4f dz = _mm_mul_ps(a.row[i], bz);
		vec4f dw = _mm_mul_ps(a.row[i], bw);

		result.row[i] = _mm_add_ps(_mm_add_ps(dx, dy), _mm_add_ps(dz, dw));
    }

    return result;
#else
#pragma error
    dest[0][0] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
    dest[0][1] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
    dest[0][2] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
    dest[0][3] = a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03;
    dest[1][0] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
    dest[1][1] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
    dest[1][2] = a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13;
    dest[1][3] = a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13;
    dest[2][0] = a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23;
    dest[2][1] = a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23;
    dest[2][2] = a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23;
    dest[2][3] = a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23;
    dest[3][0] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
    dest[3][1] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
    dest[3][2] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
    dest[3][3] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;
#endif

}
