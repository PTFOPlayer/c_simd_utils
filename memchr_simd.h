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
        else {
            return 0;
        }
        
    }
    



    return -1;
}

#endif