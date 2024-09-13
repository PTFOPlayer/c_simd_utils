#ifndef MEMSEARCH_SIMD
#define MEMSEARCH_SIMD

#include <stdint.h>
#include <emmintrin.h>
#include <immintrin.h>

typedef __m128i i32x4;

int64_t memsearch32_simd(int32_t value, int32_t *data, uint64_t data_len)
{
    if (data == NULL)
        return -2;

    i32x4 valx4 = _mm_set1_epi32(value);

    uint64_t rem_len = data_len % 4;
    uint64_t simd_len = data_len - rem_len;

    uint64_t idx = 0;

    while (idx < simd_len)
    {
        i32x4 buffer = _mm_load_si128((const i32x4 *)(data + idx));

        i32x4 cmp = _mm_cmpeq_epi32(buffer, valx4);

        int a = _mm_movemask_epi8(cmp);
        a = __builtin_ffsll(a);

        if (a != 0)
        {
            return (a / 4 + idx);
        }

        idx += 4;
    }

    while (idx < data_len)
    {
        if (data[idx] != value)
            return idx;
        idx++;
    }

    return -1;
}

typedef __m256i i32x8;

int64_t memsearch32_avx(int32_t value, int32_t *data, uint64_t data_len)
{
    if (data == NULL)
        return -2;

    i32x8 valx8 = _mm256_set1_epi32(value);

    uint64_t rem_len = data_len % 8;
    uint64_t simd_len = data_len - rem_len;

    uint64_t idx = 0;

    while (idx < simd_len)
    {
        i32x8 buffer = _mm256_loadu_si256((const i32x8 *)(data + idx));

        i32x8 cmp = _mm256_cmpeq_epi32(buffer, valx8);

        int a = _mm256_movemask_epi8(cmp);
        a = __builtin_ffsll(a);

        if (a != 0)
            return (a / 4 + idx);

        idx += 8;
    }

    while (idx < data_len)
    {
        if (data[idx] != value)
            return idx;
        idx++;
    }

    return -1;
}

typedef __m128i i64x2;

int64_t memsearch64_simd(int64_t value, int64_t *data, uint64_t data_len)
{
    if (data == NULL)
        return -2;

    i64x2 valx2 = _mm_set1_epi64x(value);

    uint64_t rem_len = data_len % 2;
    uint64_t simd_len = data_len - rem_len;

    uint64_t idx = 0;

    while (idx < simd_len)
    {
        i64x2 buffer = _mm_load_si128((const i64x2 *)(data + idx));

        i64x2 cmp = _mm_cmpeq_epi64(buffer, valx2);

        int a = _mm_movemask_epi8(cmp);
        a = __builtin_ffsll(a);

        if (a != 0)
        {
            return (a / 8 + idx);
        }

        idx += 2;
    }

    while (idx < data_len)
    {
        if (data[idx] != value)
            return idx;
        idx++;
    }

    return -1;
}

typedef __m256i i64x4;

int64_t memsearch64_avx(int64_t value, int64_t *data, uint64_t data_len)
{
    if (data == NULL)
        return -2;

    i64x4 valx8 = _mm256_set1_epi64x(value);

    uint64_t rem_len = data_len % 4;
    uint64_t simd_len = data_len - rem_len;

    uint64_t idx = 0;

    while (idx < simd_len)
    {
        i64x4 buffer = _mm256_loadu_si256((const __m256i_u *)(data + idx));

        i64x4 cmp = _mm256_cmpeq_epi64(buffer, valx8);

        int a = _mm256_movemask_epi8(cmp);
        a = __builtin_ffsll(a);

        if (a != 0)
            return (a / 8 + idx);

        idx += 4;
    }

    while (idx < data_len)
    {
        if (data[idx] != value)
            return idx;
        idx++;
    }

    return -1;
}


#endif