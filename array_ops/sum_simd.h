#ifndef ACCUMULATE_SIMD
#define ACCUMULATE_SIMD

#include <emmintrin.h>
#include <immintrin.h>
#include <stdint.h>

typedef __m128i i32x4;

int32_t fold_sum32_simd(int32_t *start, int32_t *end) {
    int64_t len = end - start;

    int64_t rem_len = len % 4;
    int64_t simd_len = len - rem_len;

    i32x4 accumulator = _mm_setzero_si128();

    int64_t idx = 0;
    while (idx < simd_len) {
        i32x4 buffer = _mm_load_si128((const i32x4 *)(start + idx));
        accumulator = _mm_add_epi32(accumulator, buffer);
        idx += 4;
    }

    int32_t sum =
        _mm_extract_epi32(accumulator, 0) + _mm_extract_epi32(accumulator, 1) +
        _mm_extract_epi32(accumulator, 2) + _mm_extract_epi32(accumulator, 3);

    while (idx < len) {
        sum += start[idx];
        idx += 1;
    }

    return sum;
}

typedef __m256i i32x8;

int32_t fold_sum32_avx(int32_t *start, int32_t *end) {
    int64_t len = end - start;

    int64_t rem_len = len % 8;
    int64_t simd_len = len - rem_len;

    i32x8 accumulator = _mm256_setzero_si256();

    int64_t idx = 0;
    while (idx < simd_len) {
        i32x8 buffer = _mm256_load_si256((const i32x8 *)(start + idx));
        accumulator = _mm256_add_epi32(accumulator, buffer);
        idx += 8;
    }

    int32_t sum = _mm256_extract_epi32(accumulator, 0) +
                  _mm256_extract_epi32(accumulator, 1) +
                  _mm256_extract_epi32(accumulator, 2) +
                  _mm256_extract_epi32(accumulator, 3) +
                  _mm256_extract_epi32(accumulator, 4) +
                  _mm256_extract_epi32(accumulator, 5) +
                  _mm256_extract_epi32(accumulator, 6) +
                  _mm256_extract_epi32(accumulator, 7);

    while (idx < len) {
        sum += start[idx];
        idx += 1;
    }

    return sum;
}

typedef __m128i i64x2;

int64_t fold_sum64_simd(int64_t *start, int64_t *end) {
    int64_t len = end - start;

    int64_t rem_len = len % 2;
    int64_t simd_len = len - rem_len;

    i64x2 accumulator = _mm_load_si128((const i64x2 *)(start));

    int64_t idx = 2;
    while (idx < simd_len) {
        i64x2 buffer = _mm_load_si128((const i64x2 *)(start + idx));
        accumulator = _mm_add_epi64(accumulator, buffer);
        idx += 2;
    }

    int32_t sum =
        _mm_extract_epi64(accumulator, 0) + _mm_extract_epi64(accumulator, 1);

    while (idx < len) {
        sum += start[idx];
        idx += 1;
    }
    return sum;
}

typedef __m256i i64x4;

int64_t fold_sum64_avx(int64_t *start, int64_t *end) {
    int64_t len = end - start;

    int64_t rem_len = len % 4;
    int64_t simd_len = len - rem_len;

    i64x4 accumulator = _mm256_setzero_si256();

    int64_t idx = 0;
    while (idx < simd_len) {
        i64x4 buffer = _mm256_load_si256((const i64x4 *)(start + idx));
        accumulator = _mm256_add_epi32(accumulator, buffer);
        idx += 4;
    }

    int64_t sum = _mm256_extract_epi64(accumulator, 0) +
                  _mm256_extract_epi64(accumulator, 1) +
                  _mm256_extract_epi64(accumulator, 2) +
                  _mm256_extract_epi64(accumulator, 3);

    while (idx < len) {
        sum += start[idx];
        idx += 1;
    }

    return sum;
}

typedef enum FoldOperation {
    SumSimd32 = 0,
    SumAvx32 = 1,
    SumSimd64 = 2,
    SumAvx64 = 3,
} FoldOperation;

void fold_sum(FoldOperation operation, void *start, void *end, void *target) {
    switch (operation) {
        case SumSimd32:
            *(int32_t *)target =
                fold_sum32_simd((int32_t *)start, (int32_t *)end);
            return;
        case SumAvx32:
            *(int32_t *)target =
                fold_sum32_avx((int32_t *)start, (int32_t *)end);
            return;
        case SumSimd64:
            *(int64_t *)target =
                fold_sum64_simd((int64_t *)start, (int64_t *)end);
            return;
        case SumAvx64:
            *(int64_t *)target =
                fold_sum64_avx((int64_t *)start, (int64_t *)end);
            return;
        default:
            break;
    }
}

#endif