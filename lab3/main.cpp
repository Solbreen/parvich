#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdint>
#include <array>

#ifdef _MSC_VER
    #include <intrin.h>
#else
    #include <cpuid.h>
#endif

class CPUInfo {
private:
    static void cpuid(uint32_t leaf, uint32_t subleaf,
                     uint32_t& eax, uint32_t& ebx,
                     uint32_t& ecx, uint32_t& edx) {
        #ifdef _MSC_VER
            int cpuInfo[4];
            __cpuidex(cpuInfo, leaf, subleaf);
            eax = cpuInfo[0];
            ebx = cpuInfo[1];
            ecx = cpuInfo[2];
            edx = cpuInfo[3];
        #else
            __cpuid_count(leaf, subleaf, eax, ebx, ecx, edx);
        #endif
    }
    
public:
    static void getEAX0() {
        uint32_t eax, ebx, ecx, edx;
        cpuid(0, 0, eax, ebx, ecx, edx);
        
        std::string vendor;
        vendor += std::string(reinterpret_cast<char*>(&ebx), 4);
        vendor += std::string(reinterpret_cast<char*>(&edx), 4);
        vendor += std::string(reinterpret_cast<char*>(&ecx), 4);

        std::cout << "============== EAX = 0 ==============" << std::endl << std::endl;
        std::cout << "Number of basic functions = " << eax << std::endl;
        std::cout << "Vendor:                     " << vendor << std::endl << std::endl;
    }

    static void getEAX1() {
        uint32_t eax, ebx, ecx, edx;
        cpuid(1, 0, eax, ebx, ecx, edx);
        
        uint32_t steppingID = eax & 0xF;
        uint32_t model = (eax >> 4) & 0xF;
        uint32_t family = (eax >> 8) & 0xF;
        uint32_t procType = (eax >> 12) & 0x3;
        uint32_t extendedModel = (eax >> 16) & 0xF;
        uint32_t extendedFamily = (eax >> 20) & 0xFF;

        std::cout << "============== EAX = 1 ==============" << std::endl << std::endl;
        
        std::cout << "EAX info = " << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << eax << "h\n";
        std::cout << "    Stepping ID      = " << steppingID << "h\n";
        std::cout << "    Model            = " << model << "h\n";
        std::cout << "    Family           = " << family << "h\n";
        std::cout << "    Processor type   = " << procType << "h\n";
        std::cout << "    Extended model   = " << extendedModel << "h\n";
        std::cout << "    Extended family  = " << std::setw(2) << std::setfill('0') << extendedFamily << "h\n\n";


        uint32_t maxLogProcNumID = (ebx >> 16) & 0xFF;
        uint32_t locAPICID = (ebx >> 24) & 0xFF;

        std::cout << "EBX info = " << std::setw(8) << std::setfill('0') << ebx << "h\n";
        std::cout << "    Maximal Logical processors number ID = " << std::setw(2) << std::setfill('0') 
            << maxLogProcNumID << "h" << std::dec << " = " << maxLogProcNumID << std::hex << std::endl;
        std::cout << "    Local APIC ID                        = " << std::setw(2) << std::setfill('0') << locAPICID << "h\n\n";


        uint32_t fpu = (edx >> 0) & 0x1;
        uint32_t tsc = (edx >> 4) & 0x1;
        uint32_t mmx = (edx >> 23) & 0x1;
        uint32_t sse = (edx >> 25) & 0x1;
        uint32_t sse2 = (edx >> 26) & 0x1;
        uint32_t htt = (edx >> 28) & 0x1;

        std::cout << "EDX info = " << std::setw(8) << std::setfill('0') << edx << "h\n";
        std::cout << "    FPU                               = " << fpu << std::endl;
        std::cout << "    TSC                               = " << tsc << std::endl;
        std::cout << "    MultyMedia Extensions (MMX)       = " << mmx << std::endl;
        std::cout << "    Streamed SIMD Extensions (SSE)    = " << sse << std::endl;
        std::cout << "    Streamed SIMD Extensions 2 (SSE2) = " << sse2 << std::endl;
        std::cout << "    Hyper-Threading (HTT)             = " << htt << std::endl << std::endl;


        uint32_t sse3 = (edx >> 0) & 0x1;
        uint32_t ssse3 = (edx >> 4) & 0x1;
        uint32_t fma3 = (edx >> 23) & 0x1;
        uint32_t sse41 = (edx >> 25) & 0x1;
        uint32_t sse42 = (edx >> 26) & 0x1;
        uint32_t avx = (edx >> 28) & 0x1;

        std::cout << "ECX info = " << std::setw(8) << std::setfill('0') << edx << "h\n";
        std::cout << "    SSE3                              = " << sse3 << std::endl;
        std::cout << "    SSSE3                             = " << ssse3 << std::endl;
        std::cout << "    FMA3                              = " << fma3 << std::endl;
        std::cout << "    SSE4.1                            = " << sse41 << std::endl;
        std::cout << "    SSE4.2                            = " << sse42 << std::endl;
        std::cout << "    AVX                               = " << avx << std::endl << std::endl;
    }
    
    static void getEAX4() {
        uint32_t eax, ebx, ecx, edx;
        cpuid(1, 0, eax, ebx, ecx, edx);
        int leaf = 4;
        int subleaf = 0;
        uint32_t procCores = 0;

        std::cout << "============== EAX = 4 ==============" << std::endl << std::endl;

        while (true) {
            cpuid(leaf, subleaf, eax, ebx, ecx, edx);
            uint32_t nn = eax;
            if (nn == 0) {
                break;
            }

            uint32_t cacheType = eax & 0x0F;
            uint32_t cacheLevel = (eax >> 5) & 0x7;
            uint32_t fa = (eax >> 9) & 0x1;
            uint32_t threadsPerCache = ((eax >> 14) & 0xFFF) + 1;
            if (subleaf == 0) {
                procCores = (eax >> 26) & 0x3F;
            }

            uint32_t cacheLineSize = (ebx & 0xFFF) + 1;
            uint32_t physicalLinePartions = ((ebx >> 12) & 0x3FF) + 1;
            uint32_t waysOfCacheAssoc = ((ebx >> 22) & 0x7FF) + 1;

            uint32_t numOfSets = ecx + 1;

            uint32_t i = (edx >> 1) & 0x1;

            uint32_t cacheSize = (cacheLineSize * physicalLinePartions * waysOfCacheAssoc * numOfSets) / 1024;

            std::cout << "ECX=" << subleaf << ": " << std::hex << std::uppercase 
            << std::setw(8) << std::setfill('0') << eax << ":" 
            << std::setw(8) << std::setfill('0') << ebx << ":" 
            << std::setw(8) << std::setfill('0') << ecx << ":" 
            << std::setw(8) << std::setfill('0') << edx 
            << std::dec << std::setfill(' ') << std::endl;

            switch (cacheType) {
                case 1:
                    std::cout << "    Data cache\n";
                    break;
                case 2:
                    std::cout << "    Instruction cache\n";
                    break;
                case 3:
                    std::cout << "    Unified cache\n";
                        break;
                default:
                    std::cout << "    I don't know\n";
                    break;
            } 
            std::cout << "    Cache level = " << cacheLevel << std::endl;
            std::cout << "    Fully Associative = " << fa << std::endl;
            std::cout << "    Threads per cache = " << threadsPerCache  << std::endl;
            std::cout << "    Cache line size = " << cacheLineSize << " Bytes\n";
            std::cout << "    Physical line partitions = " << physicalLinePartions << std::endl;
            std::cout << "    Ways of cache associativity = " << waysOfCacheAssoc << std::endl;
            if (i == 1) {
                std::cout << "    Inclusive\n";
            } else if (i == 0) {
                std::cout << "    Exclusive\n";
            }
            std::cout << "    Number of sets = " << numOfSets << std::endl;
            std::cout << "    Cache size = " << cacheSize << " KB\n\n";
            subleaf++;
        }

        std::cout << "Number of cores = " << procCores + 1 << std::endl << std::endl;
    }

    static void getEAX7() {
        uint32_t eax, ebx, ecx, edx;
        cpuid(7, 0, eax, ebx, ecx, edx);

        uint32_t avx2 = (ebx >> 5) & 0x1;
        uint32_t rtm = (ebx >> 11) & 0x1;
        uint32_t avx5121 = (ebx >> 16) & 0x1;
        uint32_t sha = (ebx >> 29) & 0x1;

        uint32_t gfni = (ecx >> 8) & 0x1;

        uint32_t amxbf16 = (edx >> 22) & 0x1;
        uint32_t amaxtile = (edx >> 24) & 0x1;
        uint32_t amaxint8 = (edx >> 25) & 0x1;

        std::cout << "============== EAX = 7, ECX = 0 ==============" << std::endl << std::endl;
        std::cout << "EAX = " << eax << " (max ECX subfunction value)\n";
        std::cout << "EBX info = " 
        << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << ebx << "h\n";
        std::cout << "    AVX2        = " << avx2 << std::endl;
        std::cout << "    RTM (TSX)   = " << rtm << std::endl;
        std::cout << "    AVX-512-f   = " << avx5121 << std::endl;
        std::cout << "    SHA         = " << sha << std::endl << std::endl;
        std::cout <<  "ECX info = " << std::setw(8) << std::setfill('0') << ecx << "h\n";
        std::cout << "    GFNI        = " << gfni << std::endl << std::endl;
        std::cout << "EDX info = " << std::setw(8) << std::setfill('0') << edx << "h\n";
        std::cout << "    AMX-BF16    = " << amxbf16 << std::endl;
        std::cout << "    AMX-TILE    = " << amaxtile << std::endl;
        std::cout << "    AMX-INT8    = " << amaxint8 << std::endl << std::endl;

        cpuid(7, 1, eax, ebx, ecx, edx);
        uint32_t amxcomplex = (edx >> 8) & 0x1;
        uint32_t avx10 = (edx >> 19) & 0x1;

        std::cout << "============== EAX = 7, ECX = 0 ==============" << std::endl << std::endl;
        std::cout << "EDX info = "  << std::setw(8) << std::setfill('0') << edx << "h\n";
        std::cout << "    AMX-COMPLEX = " << amxcomplex << std::endl;
        std::cout << "    AVX10       = " << avx10 << std::endl << std::endl;
    }

    static void getEAX16h() {
        uint32_t eax, ebx, ecx, edx;
        cpuid(22, 0, eax, ebx, ecx, edx);

        uint32_t base = eax & 0xFFFF;
        uint32_t max = ebx & 0xFFFF;
        uint32_t bus = ecx & 0xFFFF;

        std::cout << "============== EAX = 16h ==============" << std::endl << std::endl;
        std::cout << std::dec << std::setfill(' ') << "ProcessorBaseFrequency = " << base << " MHz\n";
        std::cout << "ProcessorMaxFrequency  = " << max << " MHz\n";
        std::cout << "BusReferenceFrequency  = " << bus << " MHz\n\n";
    }

    static void getEAX80000000h() {
        uint32_t eax, ebx, ecx, edx;
        cpuid(0x80000000, 0, eax, ebx, ecx, edx);

        std::cout << "============== EAX = 80000000h ==============\n\n";
        std::cout << std::hex << std::uppercase << std::setw(8) << std::setfill('0') 
        << "Maximal number of function = " << eax << "h\n\n";
    }

    static void getEAX80000001h() {
        uint32_t eax, ebx, ecx, edx;
        cpuid(0x80000001, 0, eax, ebx, ecx, edx);

        uint32_t sse4a = (ecx >> 6) & 0x1;
        uint32_t fma4 = (ecx >> 16) & 0x1;

        uint32_t d3d = (edx >> 30) & 0x1;
        uint32_t de3d = (edx >> 31) & 0x1;

        std::cout << "============== EAX = 80000001h ==============\n\n";
        std::cout << "ECX info = " << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << ecx << "h\n";
        std::cout << "    SSE4a           = " << sse4a << std::endl;
        std::cout << "    FMA4            = " << fma4 << std::endl << std::endl;
        std::cout << "EDX info = " << std::setw(8) << std::setfill('0') << edx << "h\n";
        std::cout << "    3DNow!          = " << d3d << std::endl;
        std::cout << "    Extended 3DNow! = " << de3d << std::endl << std::endl;
    }

    static void getEAX80000002h_80000004h() {
        uint32_t eax, ebx, ecx, edx;
        std::string brand;
        
        for (uint32_t i = 0x80000002; i <= 0x80000004; ++i) {
            cpuid(i, 0, eax, ebx, ecx, edx);
            brand += std::string(reinterpret_cast<char*>(&eax), 4);
            brand += std::string(reinterpret_cast<char*>(&ebx), 4);
            brand += std::string(reinterpret_cast<char*>(&ecx), 4);
            brand += std::string(reinterpret_cast<char*>(&edx), 4);
        }
        
        while (!brand.empty() && brand.back() == ' ')
            brand.pop_back();
        
        std::cout << "============== EAX = 80000002h -> 80000004h ==============" << std::endl << std::endl;
        std::cout << "CPU brand: " << brand << std::endl << std::endl;
    }
};

int main() {
    std::cout << "============== CPU Information ==============\n\n\n\n";
    CPUInfo::getEAX0();
    CPUInfo::getEAX1();
    CPUInfo::getEAX4();
    CPUInfo::getEAX7();
    CPUInfo::getEAX16h();
    CPUInfo::getEAX80000000h();
    CPUInfo::getEAX80000001h();
    CPUInfo::getEAX80000002h_80000004h();
    return 0;
}