#include <windows.h>
#include <iostream>
#include <cmath>
#include <cstdint>
#include <intrin.h>
#include <thread>
#include <chrono>

void alg() {
    // Алгоритм Subset Sum
    int n = 26;
    int target = 100;
    int a[26] = {13, 27, 5, 42, 9, 18, 33, 7, 21, 14, 3, 30, 11, 24, 6, 17, 28, 4, 19, 10, 2, 26, 8, 15, 20, 33};
    int total = 1 << n;
    for (int mask = 0; mask < total; mask++)
    {
        int sum;
        for (int i = 0; i < n; i++)
        {
            if (mask & (1 << i)) sum += a[i];
        }
        if (sum == target) return;
    }
    return;
}

void main() 
{   
    SetThreadAffinityMask(GetCurrentThread(), 1);
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    SetConsoleOutputCP(65001);
    std::cout << "Введите номер способа измерения:" << std::endl;
    std::cout << "  1) GetTickCount()" << std::endl;
    std::cout << "  2) TSC" << std::endl;
    std::cout << "  3) QueryPerformanceCounter() и QueryPerformanceFrequency()" << std::endl;

    int num_of_method = 0;
    std::cin >> num_of_method;

    switch(num_of_method) {
        case 1:
        {
            // GetTickCount()
            int StartMs = GetTickCount();
            alg();
            int EndMs = GetTickCount();
            std::cout << "Elapsed time: " << EndMs-StartMs << " ms\n";
            break;
        }
        case 2:
        {
            // TSC
            uint64_t StartTscValue, EndTscValue;
            StartTscValue = __rdtsc();
            alg();
            EndTscValue = __rdtsc();
            uint64_t start = __rdtsc();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            uint64_t end = __rdtsc();
            double s = static_cast<double>(EndTscValue - StartTscValue) / (end - start);
            std::cout << "Elapsed time: " << s << " ms\n";
            break;
        }
        case 3:
        {   
            // QueryPerformanceCounter() и QueryPerformanceFrequency()
            uint64_t StartTicks;
            QueryPerformanceCounter((LARGE_INTEGER *)&StartTicks);
            alg();
            uint64_t EndTicks;
            QueryPerformanceCounter((LARGE_INTEGER *)&EndTicks);
            uint64_t Freq;
            QueryPerformanceFrequency((LARGE_INTEGER *)&Freq);
            double s = static_cast<double>(EndTicks - StartTicks) / Freq;
            std::cout << "Elapsed time: \n" << EndTicks-StartTicks << " ticks\n" << s << " s\n";
            break;
        }
        default:
            std::cout << "Неправильный ввод" << std::endl;
            return;
    }
    return;
}
