#include <x86intrin.h>
#include <iostream>
#include <vector>
#include <cstdint>
#include <random>

unsigned long long sequentialAcc(int N, std::vector<uint32_t> &a) {

    uint32_t s = 0;
    unsigned long long start = __rdtsc();
    for (int i = 0; i < N; i+=1){
        s = s + a[i];
    }
    unsigned long long end = __rdtsc();
    unsigned long long cycles = (end - start)/N;
    return cycles;
}

unsigned long long randomAcc(int N, std::vector<uint32_t> &a) {

    uint32_t s = 0;
    std::mt19937 gen(42);
    std::uniform_int_distribution<size_t> dist(0, N - 1);
    unsigned long long start = __rdtsc();
    for (int i = 0; i < N; i+=1){
        size_t index = dist(gen);
        s = s + a[index];
    }
    unsigned long long end = __rdtsc();
    unsigned long long cycles = (end - start)/N;
    return cycles;
}

unsigned long long randomDostAcc(int N, std::vector<uint32_t> &a) {

    uint32_t s = 0;
    std::mt19937 gen(42);
    std::uniform_int_distribution<size_t> dist(0, N - 1);
    
    std::vector<size_t> index_arr(N);
    for (int i = 0; i < N; i +=1) {
        index_arr[i] = dist(gen);
    }

    unsigned long long start = __rdtsc();
    for (int i = 0; i < N; i+=1){
        size_t index = index_arr[i];
        s = s + a[index];
    }
    unsigned long long end = __rdtsc();
    unsigned long long cycles = (end - start)/N;
    return cycles;
}


int main() {
    std::vector<uint32_t> a1(524288, 1);
    std::vector<uint32_t> a2(8388608, 1);
    std::vector<uint32_t> a3(39321600, 1);
    std::vector<unsigned long long> tacts;

    int sum = 0;

    std::cout << "Какое суммирование?\n";
    std::cout << "1) Прямое\n";
    std::cout << "2) Рандомное\n";
    std::cout << "3) Рандомное с массивом\n";
    std::cin >> sum;

    int razm = 0;

    std::cout << "Какой размер?\n";
    std::cout << "1) 2 Мб\n";
    std::cout << "2) 32 Мб\n";
    std::cout << "3) 150 Мб\n";
    std::cin >> razm;

    if (razm != 1 && razm != 2 && razm != 3) {
        std::cout << "нет такого размера\n";
        return 0;
    }

    switch(sum) {
        case 1:
        {   
            if (razm == 1) {
                for (int n = 256; n <=524288; n += 256) {
                    unsigned long long o[3];
                        for (int t = 0; t < 3; t+=1) {
                            o[t] = sequentialAcc(n, a1);
                        }    
                    unsigned long long z = (o[0] + o[1] + o[2]) / 3;
                    tacts.push_back(z);
                }   
            }
            if (razm == 2) {
                for (int n = 131072; n <=8388608; n += 131072) {
                    unsigned long long o[3];
                        for (int t = 0; t < 3; t+=1) {
                            o[t] = sequentialAcc(n, a2);
                        }    
                    unsigned long long z = (o[0] + o[1] + o[2]) / 3;
                    tacts.push_back(z);
                }   
            }
            if (razm == 3) {
                for (int n = 1310720; n <=39321600; n += 1310720) {
                    unsigned long long o[3];
                        for (int t = 0; t < 3; t+=1) {
                            o[t] = sequentialAcc(n, a3);
                        }    
                    unsigned long long z = (o[0] + o[1] + o[2]) / 3;
                    tacts.push_back(z);
                }   
            }
            break;
        }
        case 2:
        {   
            if (razm == 1) {
                for (int n = 256; n <=524288; n += 256) {
                    unsigned long long o[3];
                        for (int t = 0; t < 3; t+=1) {
                            o[t] = randomAcc(n, a1);
                        }    
                    unsigned long long z = (o[0] + o[1] + o[2]) / 3;
                    tacts.push_back(z);
                }   
            }
            if (razm == 2) {
                for (int n = 131072; n <=8388608; n += 131072) {
                    unsigned long long o[3];
                        for (int t = 0; t < 3; t+=1) {
                            o[t] = randomAcc(n, a2);
                        }    
                    unsigned long long z = (o[0] + o[1] + o[2]) / 3;
                    tacts.push_back(z);
                }   
            }
            if (razm == 3) {
                for (int n = 1310720; n <=39321600; n += 1310720) {
                    unsigned long long o[3];
                        for (int t = 0; t < 3; t+=1) {
                            o[t] = randomAcc(n, a3);
                        }    
                    unsigned long long z = (o[0] + o[1] + o[2]) / 3;
                    tacts.push_back(z);
                }   
            }
            break;
        }
        case 3:
        {
            if (razm == 1) {
                for (int n = 256; n <=524288; n += 256) {
                    unsigned long long o[3];
                        for (int t = 0; t < 3; t+=1) {
                            o[t] = randomDostAcc(n, a1);
                        }    
                    unsigned long long z = (o[0] + o[1] + o[2]) / 3;
                    tacts.push_back(z);
                }   
            }
            if (razm == 2) {
                for (int n = 131072; n <=8388608; n += 131072) {
                    unsigned long long o[3];
                        for (int t = 0; t < 3; t+=1) {
                            o[t] = randomDostAcc(n, a2);
                        }    
                    unsigned long long z = (o[0] + o[1] + o[2]) / 3;
                    tacts.push_back(z);
                }   
            }
            if (razm == 3) {
                for (int n = 1310720; n <=39321600; n += 1310720) {
                    unsigned long long o[3];
                        for (int t = 0; t < 3; t+=1) {
                            o[t] = randomDostAcc(n, a3);
                        }    
                    unsigned long long z = (o[0] + o[1] + o[2]) / 3;
                    tacts.push_back(z);
                }   
            }
            break;
        }
        default:
            std::cout << "нет такого суммирования\n";
            return 0;
    }

    for (int i = 0; i < tacts.size(); i+=1) {
        std::cout << tacts[i] << ", ";
    }
    return 0;
}