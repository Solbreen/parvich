#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>

using namespace std;

extern "C" void vec_add_cuda(const float *a, const float *b, float *c, int n);

const int N = 1024 * 1024;

float a[N], b[N], c_gpu[N], c_cpu[N];

void vec_add_cpu(const float *a, const float *b, float *c, int n)
{
    for (int i = 0; i < n; ++i)
        c[i] = a[i] + b[i];
}

int main()
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < N; ++i)
    {
        a[i] = (float)(rand() % 1000) / 10.0f;
        b[i] = (float)(rand() % 1000) / 10.0f;
        c_gpu[i] = 0.0f;
        c_cpu[i] = 0.0f;
    }

    vec_add_cuda(a, b, c_gpu, N);

    vec_add_cpu(a, b, c_cpu, N);

    cout << "First 20 GPU results:\n";
    for (int i = 0; i < 20; ++i)
        cout << c_gpu[i] << " ";
    cout << "\n\n";

    int errors = 0;
    float max_diff = 0.0f;
    for (int i = 0; i < N; ++i)
    {
        float diff = fabs(c_gpu[i] - c_cpu[i]);
        if (diff > 1e-4f) ++errors;
        if (diff > max_diff) max_diff = diff;
    }

    cout << "Vector size: " << N << "\n";
    cout << "Errors (|gpu-cpu| > 1e-4): " << errors << "\n";
    cout << "Max difference: " << max_diff << "\n";

    if (errors == 0)
        cout << "\nGPU computation is CORRECT.\n";
    else
        cout << "\nGPU computation has DIFFERENCES from CPU.\n";

    cout << "\nPress Enter to exit...";
    cin.get();
    return 0;
}
