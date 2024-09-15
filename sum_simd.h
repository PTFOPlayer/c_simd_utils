#ifndef ACCUMULATE_SIMD
#define ACCUMULATE_SIMD

#include <stdint.h>
#include <emmintrin.h>
#include <immintrin.h>

typedef __m128i i32x4;

int32_t sum32_simd(int32_t *start, int32_t *end)
{
    int64_t len = end - start;

    int64_t rem_len = len % 4;
    int64_t simd_len = len - rem_len;

    i32x4 accumulator = _mm_load_si128((const i32x4 *)(start));

    int64_t idx = 4;
    while (idx < simd_len)
    {
        i32x4 buffer = _mm_load_si128((const i32x4 *)(start + idx));
        accumulator = _mm_add_epi32(accumulator, buffer);
        idx += 4;
    }

    int32_t sum =
        _mm_extract_epi32(accumulator, 0) +
        _mm_extract_epi32(accumulator, 1) +
        _mm_extract_epi32(accumulator, 2) +
        _mm_extract_epi32(accumulator, 3);

    while (idx < len)
        sum += start[idx];

    return sum;
}


typedef __m128i i64x2;

int64_t sum64_simd(int64_t *start, int64_t *end)
{
    int64_t len = end - start;

    int64_t rem_len = len % 2;
    int64_t simd_len = len - rem_len;

    i64x2 accumulator = _mm_load_si128((const i64x2 *)(start));

    int64_t idx = 2;
    while (idx < simd_len)
    {
        i64x2 buffer = _mm_load_si128((const i64x2 *)(start + idx));
        accumulator = _mm_add_epi64(accumulator, buffer);
        idx += 2;
    }

    int32_t sum =
        _mm_extract_epi64(accumulator, 0) +
        _mm_extract_epi64(accumulator, 1);

    while (idx < len)
        sum += start[idx];

    return sum;
}

#endif