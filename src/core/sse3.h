#pragma once

#include "sse2.h"

/* Define the default attributes for the functions in this file. */
#define __DEFAULT_FN_ATTRS \
  __attribute__((__always_inline__, __nodebug__, __target__("sse3"), __min_vector_width__(128)))

/// Loads data from an unaligned memory location to elements in a 128-bit
///    vector.
///
///    If the address of the data is not 16-byte aligned, the instruction may
///    read two adjacent aligned blocks of memory to retrieve the requested
///    data.
///
/// \headerfile <x86intrin.h>
///
/// This intrinsic corresponds to the <c> VLDDQU </c> instruction.
///
/// \param __p
///    A pointer to a 128-bit integer vector containing integer values.
/// \returns A 128-bit vector containing the moved values.
static __inline__ vec2di __DEFAULT_FN_ATTRS
sse3_lddqu_si128(vec2di const *__p)
{
    return (vec2di)__builtin_ia32_lddqu((char const *)__p);
}

/// Adds the even-indexed values and subtracts the odd-indexed values of
///    two 128-bit vectors of [4 x float].
///
/// \headerfile <x86intrin.h>
///
/// This intrinsic corresponds to the <c> VADDSUBPS </c> instruction.
///
/// \param __a
///    A 128-bit vector of [4 x float] containing the left source operand.
/// \param __b
///    A 128-bit vector of [4 x float] containing the right source operand.
/// \returns A 128-bit vector of [4 x float] containing the alternating sums and
///    differences of both operands.
static __inline__ vec4f __DEFAULT_FN_ATTRS
sse3_addsub_ps(vec4f __a, vec4f __b)
{
    return __builtin_ia32_addsubps((__v4sf)__a, (__v4sf)__b);
}

/// Horizontally adds the adjacent pairs of values contained in two
///    128-bit vectors of [4 x float].
///
/// \headerfile <x86intrin.h>
///
/// This intrinsic corresponds to the <c> VHADDPS </c> instruction.
///
/// \param __a
///    A 128-bit vector of [4 x float] containing one of the source operands.
///    The horizontal sums of the values are stored in the lower bits of the
///    destination.
/// \param __b
///    A 128-bit vector of [4 x float] containing one of the source operands.
///    The horizontal sums of the values are stored in the upper bits of the
///    destination.
/// \returns A 128-bit vector of [4 x float] containing the horizontal sums of
///    both operands.
static __inline__ vec4f __DEFAULT_FN_ATTRS
sse3_hadd_ps(vec4f __a, vec4f __b)
{
    return __builtin_ia32_haddps((__v4sf)__a, (__v4sf)__b);
}

/// Horizontally subtracts the adjacent pairs of values contained in two
///    128-bit vectors of [4 x float].
///
/// \headerfile <x86intrin.h>
///
/// This intrinsic corresponds to the <c> VHSUBPS </c> instruction.
///
/// \param __a
///    A 128-bit vector of [4 x float] containing one of the source operands.
///    The horizontal differences between the values are stored in the lower
///    bits of the destination.
/// \param __b
///    A 128-bit vector of [4 x float] containing one of the source operands.
///    The horizontal differences between the values are stored in the upper
///    bits of the destination.
/// \returns A 128-bit vector of [4 x float] containing the horizontal
///    differences of both operands.
static __inline__ vec4f __DEFAULT_FN_ATTRS
sse3_hsub_ps(vec4f __a, vec4f __b)
{
    return __builtin_ia32_hsubps((__v4sf)__a, (__v4sf)__b);
}

/// Moves and duplicates odd-indexed values from a 128-bit vector
///    of [4 x float] to float values stored in a 128-bit vector of
///    [4 x float].
///
/// \headerfile <x86intrin.h>
///
/// This intrinsic corresponds to the <c> VMOVSHDUP </c> instruction.
///
/// \param __a
///    A 128-bit vector of [4 x float]. \n
///    Bits [127:96] of the source are written to bits [127:96] and [95:64] of
///    the destination. \n
///    Bits [63:32] of the source are written to bits [63:32] and [31:0] of the
///    destination.
/// \returns A 128-bit vector of [4 x float] containing the moved and duplicated
///    values.
static __inline__ vec4f __DEFAULT_FN_ATTRS
sse3_movehdup_ps(vec4f __a)
{
    return __builtin_shufflevector((__v4sf)__a, (__v4sf)__a, 1, 1, 3, 3);
}

/// Duplicates even-indexed values from a 128-bit vector of
///    [4 x float] to float values stored in a 128-bit vector of [4 x float].
///
/// \headerfile <x86intrin.h>
///
/// This intrinsic corresponds to the <c> VMOVSLDUP </c> instruction.
///
/// \param __a
///    A 128-bit vector of [4 x float] \n
///    Bits [95:64] of the source are written to bits [127:96] and [95:64] of
///    the destination. \n
///    Bits [31:0] of the source are written to bits [63:32] and [31:0] of the
///    destination.
/// \returns A 128-bit vector of [4 x float] containing the moved and duplicated
///    values.
static __inline__ vec4f __DEFAULT_FN_ATTRS
sse3_moveldup_ps(vec4f __a)
{
    return __builtin_shufflevector((__v4sf)__a, (__v4sf)__a, 0, 0, 2, 2);
}

/// Adds the even-indexed values and subtracts the odd-indexed values of
///    two 128-bit vectors of [2 x double].
///
/// \headerfile <x86intrin.h>
///
/// This intrinsic corresponds to the <c> VADDSUBPD </c> instruction.
///
/// \param __a
///    A 128-bit vector of [2 x double] containing the left source operand.
/// \param __b
///    A 128-bit vector of [2 x double] containing the right source operand.
/// \returns A 128-bit vector of [2 x double] containing the alternating sums
///    and differences of both operands.
static __inline__ vec2df __DEFAULT_FN_ATTRS
sse3_addsub_pd(vec2df __a, vec2df __b)
{
    return __builtin_ia32_addsubpd((v2df)__a, (v2df)__b);
}

/// Horizontally adds the pairs of values contained in two 128-bit
///    vectors of [2 x double].
///
/// \headerfile <x86intrin.h>
///
/// This intrinsic corresponds to the <c> VHADDPD </c> instruction.
///
/// \param __a
///    A 128-bit vector of [2 x double] containing one of the source operands.
///    The horizontal sum of the values is stored in the lower bits of the
///    destination.
/// \param __b
///    A 128-bit vector of [2 x double] containing one of the source operands.
///    The horizontal sum of the values is stored in the upper bits of the
///    destination.
/// \returns A 128-bit vector of [2 x double] containing the horizontal sums of
///    both operands.
static __inline__ vec2df __DEFAULT_FN_ATTRS
sse3_hadd_pd(vec2df __a, vec2df __b)
{
    return __builtin_ia32_haddpd((v2df)__a, (v2df)__b);
}

/// Horizontally subtracts the pairs of values contained in two 128-bit
///    vectors of [2 x double].
///
/// \headerfile <x86intrin.h>
///
/// This intrinsic corresponds to the <c> VHSUBPD </c> instruction.
///
/// \param __a
///    A 128-bit vector of [2 x double] containing one of the source operands.
///    The horizontal difference of the values is stored in the lower bits of
///    the destination.
/// \param __b
///    A 128-bit vector of [2 x double] containing one of the source operands.
///    The horizontal difference of the values is stored in the upper bits of
///    the destination.
/// \returns A 128-bit vector of [2 x double] containing the horizontal
///    differences of both operands.
static __inline__ vec2df __DEFAULT_FN_ATTRS
sse3_hsub_pd(vec2df __a, vec2df __b)
{
    return __builtin_ia32_hsubpd((v2df)__a, (v2df)__b);
}

/// Moves and duplicates one double-precision value to double-precision
///    values stored in a 128-bit vector of [2 x double].
///
/// \headerfile <x86intrin.h>
///
/// \code
/// vec2df sse3_loaddup_pd(double const *dp);
/// \endcode
///
/// This intrinsic corresponds to the <c> VMOVDDUP </c> instruction.
///
/// \param dp
///    A pointer to a double-precision value to be moved and duplicated.
/// \returns A 128-bit vector of [2 x double] containing the moved and
///    duplicated values.
#define        sse3_loaddup_pd(dp)        sse3_load1_pd(dp)

/// Moves and duplicates the double-precision value in the lower bits of
///    a 128-bit vector of [2 x double] to double-precision values stored in a
///    128-bit vector of [2 x double].
///
/// \headerfile <x86intrin.h>
///
/// This intrinsic corresponds to the <c> VMOVDDUP </c> instruction.
///
/// \param __a
///    A 128-bit vector of [2 x double]. Bits [63:0] are written to bits
///    [127:64] and [63:0] of the destination.
/// \returns A 128-bit vector of [2 x double] containing the moved and
///    duplicated values.
static __inline__ vec2df __DEFAULT_FN_ATTRS
sse3_movedup_pd(vec2df __a)
{
    return __builtin_shufflevector((__v2df)__a, (__v2df)__a, 0, 0);
}

/// Establishes a linear address memory range to be monitored and puts
///    the processor in the monitor event pending state. Data stored in the
///    monitored address range causes the processor to exit the pending state.
///
/// \headerfile <x86intrin.h>
///
/// This intrinsic corresponds to the <c> MONITOR </c> instruction.
///
/// \param __p
///    The memory range to be monitored. The size of the range is determined by
///    CPUID function 0000_0005h.
/// \param __extensions
///    Optional extensions for the monitoring state.
/// \param __hints
///    Optional hints for the monitoring state.
static __inline__ void __DEFAULT_FN_ATTRS
sse3_monitor(void const *__p, unsigned __extensions, unsigned __hints)
{
    __builtin_ia32_monitor((void *)__p, __extensions, __hints);
}

/// Used with the MONITOR instruction to wait while the processor is in
///    the monitor event pending state. Data stored in the monitored address
///    range causes the processor to exit the pending state.
///
/// \headerfile <x86intrin.h>
///
/// This intrinsic corresponds to the <c> MWAIT </c> instruction.
///
/// \param __extensions
///    Optional extensions for the monitoring state, which may vary by
///    processor.
/// \param __hints
///    Optional hints for the monitoring state, which may vary by processor.
static __inline__ void __DEFAULT_FN_ATTRS
sse3_mwait(unsigned __extensions, unsigned __hints)
{
    __builtin_ia32_mwait(__extensions, __hints);
}

#undef __DEFAULT_FN_ATTRS
