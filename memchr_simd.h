#ifndef MEMCHR_SIMD
#define MEMCHR_SIMD

#include <stdint.h>
#include <emmintrin.h>

typedef __m128i i8x16;

int64_t memchr_simd(int8_t byte, int8_t *data, uint64_t data_len)
{

    i8x16 bytex16 = _mm_set1_epi8(byte);

    uint64_t rem_len = data_len % 16;
    uint64_t simd_len = data_len - rem_len;

    uint64_t idx = 0;

    while (idx < simd_len)
    {
        i8x16 buffer = _mm_load_si128((const i8x16 *)(data + idx));

        i8x16 cmp = _mm_cmpeq_epi8(buffer, bytex16);

        int a = _mm_movemask_epi8(cmp);
        a = __builtin_ffsll(a);

        if (a != 0)
        {
            return a + idx - 1;
        }

        idx += 16;
    }

    while (idx < data_len)
    {
        if (data[idx] != byte)
        {
            return idx;
        }
        else
        {
            return 0;
        }
    }

    return -1;
}

int64_t memchr_sequance_simd(int8_t *seq, int8_t *data, uint64_t seq_len, uint64_t data_len)
{

    if (seq_len > data_len)
        return -1;

    int64_t temporary;
    int64_t offset = 0;
    while (temporary = memchr_simd(seq[0], data + offset, data_len - offset))
    {
        if (temporary == -1 || ((data_len - offset) < seq_len))
            return -1;

        int8_t correct = 1;
        for (size_t i = 1; i < seq_len; i++)
            if ((data + offset)[temporary + i] != seq[i])
                correct = 0;

        if (correct)
            return temporary;

        offset += temporary;
    }

    return -1;
}
#endif