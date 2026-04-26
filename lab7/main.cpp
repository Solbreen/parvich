#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <chrono>
#include <vector>
#include <immintrin.h>


void scalar(const uint8_t* src, uint8_t* dst, int W, int H)
{
    std::memcpy(dst, src, (size_t)W * H);
    for (int y = 1; y < H - 1; ++y) {
        for (int x = 1; x < W - 1; ++x) {
            int sum = 0;
            for (int dy = -1; dy <= 1; ++dy)
                for (int dx = -1; dx <= 1; ++dx)
                    sum += src[(y + dy) * W + (x + dx)];
            dst[y * W + x] = (uint8_t)(sum / 9);
        }
    }
}

void avx2(const uint8_t* src, uint8_t* dst, int W, int H)
{
    std::memcpy(dst, src, (size_t)W * H);

    const __m256 inv9 = _mm256_set1_ps(1.0f / 9.0f);

    for (int y = 1; y < H - 1; ++y) {
        const uint8_t* row0 = src + (y - 1) * W;
        const uint8_t* row1 = src + (y    ) * W;
        const uint8_t* row2 = src + (y + 1) * W;
        uint8_t*       rowD = dst +  y      * W;

        int x = 1;
        for (; x + 16 <= W - 1; x += 16) {
            __m128i r0_l = _mm_loadu_si128((const __m128i*)(row0 + x - 1));
            __m128i r0_c = _mm_loadu_si128((const __m128i*)(row0 + x    ));
            __m128i r0_r = _mm_loadu_si128((const __m128i*)(row0 + x + 1));

            __m128i r1_l = _mm_loadu_si128((const __m128i*)(row1 + x - 1));
            __m128i r1_c = _mm_loadu_si128((const __m128i*)(row1 + x    ));
            __m128i r1_r = _mm_loadu_si128((const __m128i*)(row1 + x + 1));

            __m128i r2_l = _mm_loadu_si128((const __m128i*)(row2 + x - 1));
            __m128i r2_c = _mm_loadu_si128((const __m128i*)(row2 + x    ));
            __m128i r2_r = _mm_loadu_si128((const __m128i*)(row2 + x + 1));

            __m256i v0_l = _mm256_cvtepu8_epi16(r0_l);
            __m256i v0_c = _mm256_cvtepu8_epi16(r0_c);
            __m256i v0_r = _mm256_cvtepu8_epi16(r0_r);
            __m256i v1_l = _mm256_cvtepu8_epi16(r1_l);
            __m256i v1_c = _mm256_cvtepu8_epi16(r1_c);
            __m256i v1_r = _mm256_cvtepu8_epi16(r1_r);
            __m256i v2_l = _mm256_cvtepu8_epi16(r2_l);
            __m256i v2_c = _mm256_cvtepu8_epi16(r2_c);
            __m256i v2_r = _mm256_cvtepu8_epi16(r2_r);

            __m256i s = _mm256_add_epi16(v0_l, v0_c);
            s = _mm256_add_epi16(s, v0_r);
            s = _mm256_add_epi16(s, v1_l);
            s = _mm256_add_epi16(s, v1_c);
            s = _mm256_add_epi16(s, v1_r);
            s = _mm256_add_epi16(s, v2_l);
            s = _mm256_add_epi16(s, v2_c);
            s = _mm256_add_epi16(s, v2_r);

            __m128i s_lo = _mm256_extracti128_si256(s, 0);
            __m128i s_hi = _mm256_extracti128_si256(s, 1);

            __m256i s_lo_i32 = _mm256_cvtepu16_epi32(s_lo);
            __m256i s_hi_i32 = _mm256_cvtepu16_epi32(s_hi);

            __m256 f_lo = _mm256_cvtepi32_ps(s_lo_i32);
            __m256 f_hi = _mm256_cvtepi32_ps(s_hi_i32);

            f_lo = _mm256_mul_ps(f_lo, inv9);
            f_hi = _mm256_mul_ps(f_hi, inv9);

            __m256i out_lo = _mm256_cvttps_epi32(f_lo);
            __m256i out_hi = _mm256_cvttps_epi32(f_hi);

            __m256i out16 = _mm256_packus_epi32(out_lo, out_hi);
            out16 = _mm256_permute4x64_epi64(out16, 0b11011000);

            __m128i out16_lo = _mm256_extracti128_si256(out16, 0);
            __m128i out16_hi = _mm256_extracti128_si256(out16, 1);
            __m128i out8 = _mm_packus_epi16(out16_lo, out16_hi);

            _mm_storeu_si128((__m128i*)(rowD + x), out8);
        }

        for (; x < W - 1; ++x) {
            int sum = 0;
            for (int dy = -1; dy <= 1; ++dy)
                for (int dx = -1; dx <= 1; ++dx)
                    sum += src[(y + dy) * W + (x + dx)];
            rowD[x] = (uint8_t)(sum / 9);
        }
    }
}

bool save_pgm(const char* path, const uint8_t* img, int W, int H)
{
    FILE* f = std::fopen(path, "wb");
    if (!f) return false;
    std::fprintf(f, "P5\n%d %d\n255\n", W, H);
    std::fwrite(img, 1, (size_t)W * H, f);
    std::fclose(f);
    return true;
}

int main()
{
    const int W = 1920;
    const int H = 1080;
    const int N = W * H;

    std::vector<uint8_t> src(N), dst_scalar(N), dst_simd(N);

    uint32_t seed = 1234567u;
    for (int i = 0; i < N; ++i) {
        seed = seed * 1664525u + 1013904223u;
        src[i] = (uint8_t)(seed >> 24);
    }

    const int RUNS = 5;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < RUNS; ++r)
        scalar(src.data(), dst_scalar.data(), W, H);
    auto t1 = std::chrono::high_resolution_clock::now();

    auto t2 = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < RUNS; ++r)
        avx2(src.data(), dst_simd.data(), W, H);
    auto t3 = std::chrono::high_resolution_clock::now();

    double t_scalar = std::chrono::duration<double, std::milli>(t1 - t0).count() / RUNS;
    double t_simd   = std::chrono::duration<double, std::milli>(t3 - t2).count() / RUNS;

    int diff_count = 0;
    int max_diff   = 0;
    for (int i = 0; i < N; ++i) {
        int d = std::abs((int)dst_scalar[i] - (int)dst_simd[i]);
        if (d != 0) {
            ++diff_count;
        }
        if (d > max_diff) {
            max_diff = d;
        }
    }

    std::printf("Размер изображения:   %d x %d (%d пикселей)\n", W, H, N);
    std::printf("\nВремя выполнения (среднее):\n");
    std::printf("  Скалярная версия:   %8.3f мс\n", t_scalar);
    std::printf("  AVX2 версия:        %8.3f мс\n", t_simd);
    std::printf("  Ускорение:          %8.2fx\n", t_scalar / t_simd);
    std::printf("\nКорректность (сравнение скаляр vs simd):\n");
    std::printf("  Различающихся пикселей: %d из %d\n", diff_count, N);

    save_pgm("./source.pgm",      src.data(),         W, H);
    save_pgm("./blur_scalar.pgm", dst_scalar.data(),  W, H);
    save_pgm("./blur_avx2.pgm",   dst_simd.data(),    W, H);

    return 0;
}