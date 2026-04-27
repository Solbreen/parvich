#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <cuda.h>
#include <cuda_runtime.h>
#include <windows.h>

using namespace std;

const size_t BLOCK_SIZE = 100ull * 1024 * 1024;
const int RUNS = 5;

inline double bandwidth_gb_s(size_t bytes, float ms)
{
    return (double)bytes / (1024.0 * 1024.0 * 1024.0) / (ms / 1000.0);
}

double measure_cuda_memcpy(void *dst, const void *src, size_t size,
                           cudaMemcpyKind kind)
{
    cudaEvent_t e_start, e_stop;
    cudaEventCreate(&e_start);
    cudaEventCreate(&e_stop);

    float total_ms = 0.0f;
    for (int r = 0; r < RUNS; ++r)
    {
        cudaEventRecord(e_start);
        cudaMemcpy(dst, src, size, kind);
        cudaEventRecord(e_stop);
        cudaEventSynchronize(e_stop);

        float ms = 0.0f;
        cudaEventElapsedTime(&ms, e_start, e_stop);
        total_ms += ms;
    }

    cudaEventDestroy(e_start);
    cudaEventDestroy(e_stop);

    return bandwidth_gb_s(size, total_ms / RUNS);
}

double measure_host_memcpy(void *dst, const void *src, size_t size)
{
    cudaEvent_t e_start, e_stop;
    cudaEventCreate(&e_start);
    cudaEventCreate(&e_stop);

    float total_ms = 0.0f;
    for (int r = 0; r < RUNS; ++r)
    {
        cudaEventRecord(e_start);
        memcpy(dst, src, size);
        cudaEventRecord(e_stop);
        cudaEventSynchronize(e_stop);

        float ms = 0.0f;
        cudaEventElapsedTime(&ms, e_start, e_stop);
        total_ms += ms;
    }

    cudaEventDestroy(e_start);
    cudaEventDestroy(e_stop);

    return bandwidth_gb_s(size, total_ms / RUNS);
}

bool verify(const void *src, const void *dst, size_t size)
{
    return memcmp(src, dst, size) == 0;
}

int main()
{
    SetConsoleOutputCP(65001);
    cout << "Тест пропускной способности памяти CUDA (размер блока = "
         << (BLOCK_SIZE >> 20) << " МБ)\n\n";

    int device_count = 0;
    cudaGetDeviceCount(&device_count);
    cout << device_count << " CUDA найдена\n";
    if (device_count > 0)
    {
        cudaDeviceProp dp;
        cudaGetDeviceProperties(&dp, 0);
        cout << "GPU 0: " << dp.name << "\n\n";
    }

    cout << fixed << setprecision(3);

    cout << "RAM <-> RAM\n";
    char *h_src = (char *)malloc(BLOCK_SIZE);
    char *h_dst = (char *)malloc(BLOCK_SIZE);
    for (size_t i = 0; i < BLOCK_SIZE; ++i) h_src[i] = (char)(i & 0xFF);
    memset(h_dst, 0, BLOCK_SIZE);

    double bw = measure_host_memcpy(h_dst, h_src, BLOCK_SIZE);
    cout << "  RAM -> RAM: " << bw << " ГБ/с   "
         << (verify(h_src, h_dst, BLOCK_SIZE) ? "[OK]" : "[FAIL]") << "\n\n";

    cout << "RAM <-> GPU (обычная host-память)\n";
    char *d_buf1 = NULL;
    cudaMalloc((void **)&d_buf1, BLOCK_SIZE);

    bw = measure_cuda_memcpy(d_buf1, h_src, BLOCK_SIZE, cudaMemcpyHostToDevice);
    cout << "  RAM -> GPU: " << bw << " ГБ/с\n";

    memset(h_dst, 0, BLOCK_SIZE);
    bw = measure_cuda_memcpy(h_dst, d_buf1, BLOCK_SIZE, cudaMemcpyDeviceToHost);
    cout << "  GPU -> RAM: " << bw << " ГБ/с   "
         << (verify(h_src, h_dst, BLOCK_SIZE) ? "[OK]" : "[FAIL]") << "\n\n";


    cout << "RAM <-> GPU (page-locked / pinned host-память)\n";
    char *h_pinned_src = NULL;
    char *h_pinned_dst = NULL;
    cudaMallocHost((void **)&h_pinned_src, BLOCK_SIZE);
    cudaMallocHost((void **)&h_pinned_dst, BLOCK_SIZE);
    memcpy(h_pinned_src, h_src, BLOCK_SIZE);
    memset(h_pinned_dst, 0, BLOCK_SIZE);

    bw = measure_cuda_memcpy(d_buf1, h_pinned_src, BLOCK_SIZE,
                             cudaMemcpyHostToDevice);
    cout << "  RAM -> GPU:   " << bw << " ГБ/с\n";

    bw = measure_cuda_memcpy(h_pinned_dst, d_buf1, BLOCK_SIZE,
                             cudaMemcpyDeviceToHost);
    cout << "  HPU -> RAM:   " << bw << " ГБ/с   "
         << (verify(h_pinned_src, h_pinned_dst, BLOCK_SIZE) ? "[OK]" : "[FAIL]")
         << "\n\n";

    cout << "GPU <-> GPU\n";
    char *d_buf2 = NULL;
    cudaMalloc((void **)&d_buf2, BLOCK_SIZE);

    bw = measure_cuda_memcpy(d_buf2, d_buf1, BLOCK_SIZE, cudaMemcpyDeviceToDevice);

    memset(h_dst, 0, BLOCK_SIZE);
    cudaMemcpy(h_dst, d_buf2, BLOCK_SIZE, cudaMemcpyDeviceToHost);
    cout << "  GPU -> GPU:          " << bw << " ГБ/с   "
         << (verify(h_src, h_dst, BLOCK_SIZE) ? "[OK]" : "[FAIL]") << "\n\n";


    cudaFree(d_buf1);
    cudaFree(d_buf2);
    cudaFreeHost(h_pinned_src);
    cudaFreeHost(h_pinned_dst);
    free(h_src);
    free(h_dst);

    return 0;
}
