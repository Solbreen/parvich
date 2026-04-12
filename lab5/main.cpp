#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <immintrin.h>
#include <iostream>
#include <windows.h>

#define N 1000000
#define VMAX 255
#define REPEAT 10

__declspec(align(16)) int8_t a[N];
__declspec(align(16)) uint8_t res_scalar[N];
__declspec(align(16)) uint8_t res_sse_scalar[N];
__declspec(align(16)) uint8_t res_sse_vector[N];


void scalar_normalize(int8_t* src, uint8_t* dst, int n, int8_t min_val, int8_t max_val) {
    float range = (float)(max_val - min_val);
    for (int i = 0; i < n; i++) {
        float norm = ((float)(src[i] - min_val) * VMAX) / range;
        if (norm < 0) norm = 0;
        if (norm > VMAX) norm = VMAX;
        dst[i] = (uint8_t)(norm + 0.5f);
    }
}

void sse2_scalar_normalize(int8_t* src, uint8_t* dst, int n, int8_t min_val, int8_t max_val) {
    float range = (float)(max_val - min_val);
    float vmax_f = (float)VMAX;
    
    for (int i = 0; i < n; i++) {
        __m128i v_src = _mm_cvtsi32_si128((int)src[i]);
        __m128 v_float = _mm_cvtepi32_ps(_mm_cvtepi8_epi32(v_src));
        __m128 v_min = _mm_set_ss((float)min_val);
        __m128 v_range = _mm_set_ss(range);
        __m128 v_vmax = _mm_set_ss(vmax_f);
        
        v_float = _mm_sub_ss(v_float, v_min);
        v_float = _mm_mul_ss(v_float, v_vmax);
        v_float = _mm_div_ss(v_float, v_range);
        
        int val = (int)_mm_cvtss_si32(v_float);
        if (val < 0) val = 0;
        if (val > VMAX) val = VMAX;
        dst[i] = (uint8_t)val;
    }
}

void sse2_vector_normalize(int8_t* src, uint8_t* dst, int n, int8_t min_val, int8_t max_val) {
    float range = (float)(max_val - min_val);
    float scale = (float)VMAX / range;
    
    __m128 v_scale = _mm_set1_ps(scale);
    __m128 v_half = _mm_set1_ps(0.5f);
    __m128 v_max_ps = _mm_set1_ps((float)VMAX);
    __m128 v_min_ps = _mm_set1_ps(0.0f);
    
    __m128i v_min_epi16 = _mm_set1_epi16((short)min_val);
    
    for (int i = 0; i < n; i += 16) {
        __m128i v_src = _mm_load_si128((__m128i*)&src[i]);
        
        __m128i v_low = _mm_unpacklo_epi8(v_src, _mm_setzero_si128());
        __m128i v_high = _mm_unpackhi_epi8(v_src, _mm_setzero_si128());
        
        v_low = _mm_sub_epi16(v_low, v_min_epi16);
        v_high = _mm_sub_epi16(v_high, v_min_epi16);

        __m128i v_low_lo = _mm_unpacklo_epi16(v_low, _mm_setzero_si128());
        __m128i v_low_hi = _mm_unpackhi_epi16(v_low, _mm_setzero_si128());
        __m128i v_high_lo = _mm_unpacklo_epi16(v_high, _mm_setzero_si128());
        __m128i v_high_hi = _mm_unpackhi_epi16(v_high, _mm_setzero_si128());
        
        __m128 v_float1 = _mm_cvtepi32_ps(v_low_lo);
        __m128 v_float2 = _mm_cvtepi32_ps(v_low_hi);
        __m128 v_float3 = _mm_cvtepi32_ps(v_high_lo);
        __m128 v_float4 = _mm_cvtepi32_ps(v_high_hi);
        
        v_float1 = _mm_mul_ps(v_float1, v_scale);
        v_float2 = _mm_mul_ps(v_float2, v_scale);
        v_float3 = _mm_mul_ps(v_float3, v_scale);
        v_float4 = _mm_mul_ps(v_float4, v_scale);
        
        v_float1 = _mm_add_ps(v_float1, v_half);
        v_float2 = _mm_add_ps(v_float2, v_half);
        v_float3 = _mm_add_ps(v_float3, v_half);
        v_float4 = _mm_add_ps(v_float4, v_half);
        
        v_float1 = _mm_max_ps(v_float1, v_min_ps);
        v_float1 = _mm_min_ps(v_float1, v_max_ps);
        v_float2 = _mm_max_ps(v_float2, v_min_ps);
        v_float2 = _mm_min_ps(v_float2, v_max_ps);
        v_float3 = _mm_max_ps(v_float3, v_min_ps);
        v_float3 = _mm_min_ps(v_float3, v_max_ps);
        v_float4 = _mm_max_ps(v_float4, v_min_ps);
        v_float4 = _mm_min_ps(v_float4, v_max_ps);
        
        __m128i v_int1 = _mm_cvttps_epi32(v_float1);
        __m128i v_int2 = _mm_cvttps_epi32(v_float2);
        __m128i v_int3 = _mm_cvttps_epi32(v_float3);
        __m128i v_int4 = _mm_cvttps_epi32(v_float4);
        
        __m128i v_word1 = _mm_packs_epi32(v_int1, v_int2);
        __m128i v_word2 = _mm_packs_epi32(v_int3, v_int4);
        
        __m128i v_byte = _mm_packus_epi16(v_word1, v_word2);
        
        _mm_store_si128((__m128i*)&dst[i], v_byte);
    }
}

double measure_time(void (*func)(int8_t*, uint8_t*, int, int8_t, int8_t), 
                    int8_t* src, uint8_t* dst, int n, 
                    int8_t min_val, int8_t max_val, const char* name) {
    clock_t start = clock();
    for (int r = 0; r < REPEAT; r++) {
        func(src, dst, n, min_val, max_val);
    }
    clock_t end = clock();
    double time_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0 / REPEAT;
    printf("%-30s: %.3f ms\n", name, time_ms);
    return time_ms;
}

void scalar_normalize_unroll2(int8_t* src, uint8_t* dst, int n, int8_t min_val, int8_t max_val) {
    float range = (float)(max_val - min_val);
    int i;
    for (i = 0; i <= n - 2; i += 2) {
        float norm0 = ((float)(src[i] - min_val) * VMAX) / range;
        float norm1 = ((float)(src[i+1] - min_val) * VMAX) / range;
        if (norm0 < 0) norm0 = 0;
        if (norm0 > VMAX) norm0 = VMAX;
        if (norm1 < 0) norm1 = 0;
        if (norm1 > VMAX) norm1 = VMAX;
        dst[i] = (uint8_t)(norm0 + 0.5f);
        dst[i+1] = (uint8_t)(norm1 + 0.5f);
    }
    for (; i < n; i++) {
        float norm = ((float)(src[i] - min_val) * VMAX) / range;
        if (norm < 0) norm = 0;
        if (norm > VMAX) norm = VMAX;
        dst[i] = (uint8_t)(norm + 0.5f);
    }
}

int main() {
    SetConsoleOutputCP(65001);
    for (int i = 0; i < N; i++) {
        a[i] = (int8_t)(-128 + rand() % 256);
    }
    
    int8_t min_val = a[0], max_val = a[0];
    for (int i = 1; i < N; i++) {
        if (a[i] < min_val) min_val = a[i];
        if (a[i] > max_val) max_val = a[i];
    }
    
    scalar_normalize(a, res_scalar, N, min_val, max_val);
    measure_time(scalar_normalize, a, res_scalar, N, min_val, max_val, "1. Scalar (Debug/Release)");
    
    sse2_scalar_normalize(a, res_sse_scalar, N, min_val, max_val);
    measure_time(sse2_scalar_normalize, a, res_sse_scalar, N, min_val, max_val, "2. SSE2 scalar commands");
    
    sse2_vector_normalize(a, res_sse_vector, N, min_val, max_val);
    measure_time(sse2_vector_normalize, a, res_sse_vector, N, min_val, max_val, "3. SSE2 vector/horizontal");
    
    printf("\n=== Развёртка циклов скалярная на 2 итерации ===\n");
    measure_time(scalar_normalize_unroll2, a, res_scalar, N, min_val, max_val, "Unroll 2");
    
    return 0;
}