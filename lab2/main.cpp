#include <vector>
#include <random>
#include <iostream>
#include <cmath>
#include <windows.h>


// Вспомогательные функции
std::vector<std::vector<double>> generator(int size, double l, double r)
{
    std::vector<std::vector<double>> a(size, std::vector<double>(size));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(l, r);

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            a[i][j] = dist(gen);

    return a;
}

void transpose(std::vector<std::vector<double>>& mat)
{
    int n = mat.size();

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            std::swap(mat[i][j], mat[j][i]);
}

void mat(const std::vector<std::vector<double>>& c, int size)
{   
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            std::cout << c[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\n\n";
}

bool equal(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b, double eps = 1e-9)
{
    if (a.size() != b.size()) 
    {
        return false;
    }

    for (size_t i = 0; i < a.size(); i++)
    {
        if (a[i].size() != b[i].size())
        {
            return false;
        }

        for (size_t j = 0; j < a[i].size(); j++)
        {
            if (std::fabs(a[i][j] - b[i][j]) > eps)
                return false;
        }
    }
    return true;
}
////////////////////////////////////////////////////////////

// Алгоритмы умножения
std::vector<std::vector<double>> classicMultiply(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (size_t i=0; i<size; i++)
    {
        for (size_t j=0; j<size; j++)
        {
            double s = 0;
            for (size_t k=0; k<size; k++)
            {
                s += a[i][k]*b[k][j];
            }
            c[i][j] = s;
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for classicMultiply: " << (static_cast<double>(EndMs - StartMs) / 1000) << " seconds;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> withT_transposeMultiply(const std::vector<std::vector<double>>& a, std::vector<std::vector<double>> b)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    transpose(b);
    for (size_t i=0; i<size; i++)
    {
        for (size_t j=0; j<size; j++)
        {
            double s = 0;
            for (size_t k=0; k<size; k++)
            {
                s += a[i][k]*b[j][k];
            }
            c[i][j] = s;
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for transposeMultiply with transpose time: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> withoutT_transposeMultiply(const std::vector<std::vector<double>>& a, std::vector<std::vector<double>> b)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    transpose(b);
    int StartMs = GetTickCount();
    for (size_t i=0; i<size; i++)
    {
        for (size_t j=0; j<size; j++)
        {
            double s = 0;
            for (size_t k=0; k<size; k++)
            {
                s += a[i][k]*b[j][k];
            }
            c[i][j] = s;
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for transposeMultiply without transpose time: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> buffMultiply1(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (size_t j = 0; j < size; j++)
    {   
        std::vector<double> tmp(size);
        for (size_t k=0; k<size; k++)
        {
            tmp[k] = b[k][j];
        }   
        for (size_t i=0; i<size; i++)
        {
            double s = 0;
            for (size_t k=0; k<size; k++)
            {
                s += a[i][k] * tmp[k];
            }
            c[i][j] = s;
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for buffMultiply with M = 1: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> buffMultiply2(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (size_t j = 0; j < size; j++)
    {   
        std::vector<double> tmp(size);
        for (size_t k=0; k<size; k++)
        {
            tmp[k] = b[k][j];
        }   
        for (size_t i=0; i<size; i++)
        {
            double s1 = 0;
            double s2 = 0;
            for (size_t k=0; k<size; k+=2)
            {
                s1 += a[i][k] * tmp[k];
                s2 += a[i][k+1] * tmp[k+1];
            }
            c[i][j] = s1 + s2;
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for buffMultiply with M = 2: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> buffMultiply4(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (size_t j = 0; j < size; j++)
    {   
        std::vector<double> tmp(size);
        for (size_t k=0; k<size; k++)
        {
            tmp[k] = b[k][j];
        }   
        for (size_t i=0; i<size; i++)
        {
            double s1 = 0;
            double s2 = 0;
            double s3 = 0;
            double s4 = 0;
            for (size_t k=0; k<size; k+=4)
            {
                s1 += a[i][k] * tmp[k];
                s2 += a[i][k+1] * tmp[k+1];
                s3 += a[i][k+2] * tmp[k+2];
                s4 += a[i][k+3] * tmp[k+3];
            }
            c[i][j] = s1 + s2 + s3 + s4;
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for buffMultiply with M = 4: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> buffMultiply8(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (size_t j = 0; j < size; j++)
    {   
        std::vector<double> tmp(size);
        for (size_t k=0; k<size; k++)
        {
            tmp[k] = b[k][j];
        }   
        for (size_t i=0; i<size; i++)
        {
            double s1 = 0;
            double s2 = 0;
            double s3 = 0;
            double s4 = 0;
            double s5 = 0;
            double s6 = 0;
            double s7 = 0;
            double s8 = 0;
            for (size_t k=0; k<size; k+=8)
            {
                s1 += a[i][k] * tmp[k];
                s2 += a[i][k+1] * tmp[k+1];
                s3 += a[i][k+2] * tmp[k+2];
                s4 += a[i][k+3] * tmp[k+3];
                s5 += a[i][k+4] * tmp[k+4];
                s6 += a[i][k+5] * tmp[k+5];
                s7 += a[i][k+6] * tmp[k+6];
                s8 += a[i][k+7] * tmp[k+7];
            }
            c[i][j] = s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8;
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for buffMultiply with M = 8: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> buffMultiply16(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (size_t j = 0; j < size; j++)
    {   
        std::vector<double> tmp(size);
        for (size_t k=0; k<size; k++)
        {
            tmp[k] = b[k][j];
        }   
        for (size_t i=0; i<size; i++)
        {
            double s1 = 0;
            double s2 = 0;
            double s3 = 0;
            double s4 = 0;
            double s5 = 0;
            double s6 = 0;
            double s7 = 0;
            double s8 = 0;
            double s9 = 0;
            double s10 = 0;
            double s11 = 0;
            double s12 = 0;
            double s13 = 0;
            double s14 = 0;
            double s15 = 0;
            double s16 = 0;
            for (size_t k=0; k<size; k+=16)
            {
                s1 += a[i][k] * tmp[k];
                s2 += a[i][k+1] * tmp[k+1];
                s3 += a[i][k+2] * tmp[k+2];
                s4 += a[i][k+3] * tmp[k+3];
                s5 += a[i][k+4] * tmp[k+4];
                s6 += a[i][k+5] * tmp[k+5];
                s7 += a[i][k+6] * tmp[k+6];
                s8 += a[i][k+7] * tmp[k+7];
                s9 += a[i][k+8] * tmp[k+8];
                s10 += a[i][k+9] * tmp[k+9];
                s11 += a[i][k+10] * tmp[k+10];
                s12 += a[i][k+11] * tmp[k+11];
                s13 += a[i][k+12] * tmp[k+12];
                s14 += a[i][k+13] * tmp[k+13];
                s15 += a[i][k+14] * tmp[k+14];
                s16 += a[i][k+15] * tmp[k+15];
            }
            c[i][j] = s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9 + s10 + s11 + s12 + s13 + s14 + s15 + s16;
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for buffMultiply with M = 16: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> buffMultiply32(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (size_t j = 0; j < size; j++)
    {   
        std::vector<double> tmp(size);
        for (size_t k=0; k<size; k++)
        {
            tmp[k] = b[k][j];
        }   
        for (size_t i=0; i<size; i++)
        {
            double s1 = 0;
            double s2 = 0;
            double s3 = 0;
            double s4 = 0;
            double s5 = 0;
            double s6 = 0;
            double s7 = 0;
            double s8 = 0;
            double s9 = 0;
            double s10 = 0;
            double s11 = 0;
            double s12 = 0;
            double s13 = 0;
            double s14 = 0;
            double s15 = 0;
            double s16 = 0;
            double s17 = 0;
            double s18 = 0;
            double s19 = 0;
            double s20 = 0;
            double s21 = 0;
            double s22 = 0;
            double s23 = 0;
            double s24 = 0;
            double s25 = 0;
            double s26 = 0;
            double s27 = 0;
            double s28 = 0;
            double s29 = 0;
            double s30 = 0;
            double s31 = 0;
            double s32 = 0;
            for (size_t k=0; k<size; k+=32)
            {
                s1 += a[i][k] * tmp[k];
                s2 += a[i][k+1] * tmp[k+1];
                s3 += a[i][k+2] * tmp[k+2];
                s4 += a[i][k+3] * tmp[k+3];
                s5 += a[i][k+4] * tmp[k+4];
                s6 += a[i][k+5] * tmp[k+5];
                s7 += a[i][k+6] * tmp[k+6];
                s8 += a[i][k+7] * tmp[k+7];
                s9 += a[i][k+8] * tmp[k+8];
                s10 += a[i][k+9] * tmp[k+9];
                s11 += a[i][k+10] * tmp[k+10];
                s12 += a[i][k+11] * tmp[k+11];
                s13 += a[i][k+12] * tmp[k+12];
                s14 += a[i][k+13] * tmp[k+13];
                s15 += a[i][k+14] * tmp[k+14];
                s16 += a[i][k+15] * tmp[k+15];

                s17 += a[i][k+16] * tmp[k+16];
                s18 += a[i][k+17] * tmp[k+17];
                s19 += a[i][k+18] * tmp[k+18];
                s20 += a[i][k+19] * tmp[k+19];
                s21 += a[i][k+20] * tmp[k+20];
                s22 += a[i][k+21] * tmp[k+21];
                s23 += a[i][k+22] * tmp[k+22];
                s24 += a[i][k+23] * tmp[k+23];
                s25 += a[i][k+24] * tmp[k+24];
                s26 += a[i][k+25] * tmp[k+25];
                s27 += a[i][k+26] * tmp[k+26];
                s28 += a[i][k+27] * tmp[k+27];
                s29 += a[i][k+28] * tmp[k+28];
                s30 += a[i][k+29] * tmp[k+29];
                s31 += a[i][k+30] * tmp[k+30];
                s32 += a[i][k+31] * tmp[k+31];
            }
            c[i][j] = s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9 + s10 + s11 + s12 + s13 + s14 + s15 + s16 + 
            s17 + s18 + s19 + s20 + s21 + s22 + s23 + s24 + s25 + s26 + s27 + s28 + s29 + s30 + s31 + s32;
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for buffMultiply with M = 32: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> buffMultiply64(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();

    for (size_t j = 0; j < size; j++)
    {
        std::vector<double> tmp(size);
        for (size_t k = 0; k < size; k++)
        {
            tmp[k] = b[k][j];
        }

        for (size_t i = 0; i < size; i++)
        {
            double s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0, s6 = 0, s7 = 0, s8 = 0;
            double s9 = 0, s10 = 0, s11 = 0, s12 = 0, s13 = 0, s14 = 0, s15 = 0, s16 = 0;
            double s17 = 0, s18 = 0, s19 = 0, s20 = 0, s21 = 0, s22 = 0, s23 = 0, s24 = 0;
            double s25 = 0, s26 = 0, s27 = 0, s28 = 0, s29 = 0, s30 = 0, s31 = 0, s32 = 0;
            double s33 = 0, s34 = 0, s35 = 0, s36 = 0, s37 = 0, s38 = 0, s39 = 0, s40 = 0;
            double s41 = 0, s42 = 0, s43 = 0, s44 = 0, s45 = 0, s46 = 0, s47 = 0, s48 = 0;
            double s49 = 0, s50 = 0, s51 = 0, s52 = 0, s53 = 0, s54 = 0, s55 = 0, s56 = 0;
            double s57 = 0, s58 = 0, s59 = 0, s60 = 0, s61 = 0, s62 = 0, s63 = 0, s64 = 0;

            for (size_t k = 0; k < size; k += 64)
            {
                s1 += a[i][k]   * tmp[k];
                s2 += a[i][k+1] * tmp[k+1];
                s3 += a[i][k+2] * tmp[k+2];
                s4 += a[i][k+3] * tmp[k+3];
                s5 += a[i][k+4] * tmp[k+4];
                s6 += a[i][k+5] * tmp[k+5];
                s7 += a[i][k+6] * tmp[k+6];
                s8 += a[i][k+7] * tmp[k+7];
                s9 += a[i][k+8] * tmp[k+8];
                s10 += a[i][k+9] * tmp[k+9];
                s11 += a[i][k+10] * tmp[k+10];
                s12 += a[i][k+11] * tmp[k+11];
                s13 += a[i][k+12] * tmp[k+12];
                s14 += a[i][k+13] * tmp[k+13];
                s15 += a[i][k+14] * tmp[k+14];
                s16 += a[i][k+15] * tmp[k+15];
                s17 += a[i][k+16] * tmp[k+16];
                s18 += a[i][k+17] * tmp[k+17];
                s19 += a[i][k+18] * tmp[k+18];
                s20 += a[i][k+19] * tmp[k+19];
                s21 += a[i][k+20] * tmp[k+20];
                s22 += a[i][k+21] * tmp[k+21];
                s23 += a[i][k+22] * tmp[k+22];
                s24 += a[i][k+23] * tmp[k+23];
                s25 += a[i][k+24] * tmp[k+24];
                s26 += a[i][k+25] * tmp[k+25];
                s27 += a[i][k+26] * tmp[k+26];
                s28 += a[i][k+27] * tmp[k+27];
                s29 += a[i][k+28] * tmp[k+28];
                s30 += a[i][k+29] * tmp[k+29];
                s31 += a[i][k+30] * tmp[k+30];
                s32 += a[i][k+31] * tmp[k+31];
                s33 += a[i][k+32] * tmp[k+32];
                s34 += a[i][k+33] * tmp[k+33];
                s35 += a[i][k+34] * tmp[k+34];
                s36 += a[i][k+35] * tmp[k+35];
                s37 += a[i][k+36] * tmp[k+36];
                s38 += a[i][k+37] * tmp[k+37];
                s39 += a[i][k+38] * tmp[k+38];
                s40 += a[i][k+39] * tmp[k+39];
                s41 += a[i][k+40] * tmp[k+40];
                s42 += a[i][k+41] * tmp[k+41];
                s43 += a[i][k+42] * tmp[k+42];
                s44 += a[i][k+43] * tmp[k+43];
                s45 += a[i][k+44] * tmp[k+44];
                s46 += a[i][k+45] * tmp[k+45];
                s47 += a[i][k+46] * tmp[k+46];
                s48 += a[i][k+47] * tmp[k+47];
                s49 += a[i][k+48] * tmp[k+48];
                s50 += a[i][k+49] * tmp[k+49];
                s51 += a[i][k+50] * tmp[k+50];
                s52 += a[i][k+51] * tmp[k+51];
                s53 += a[i][k+52] * tmp[k+52];
                s54 += a[i][k+53] * tmp[k+53];
                s55 += a[i][k+54] * tmp[k+54];
                s56 += a[i][k+55] * tmp[k+55];
                s57 += a[i][k+56] * tmp[k+56];
                s58 += a[i][k+57] * tmp[k+57];
                s59 += a[i][k+58] * tmp[k+58];
                s60 += a[i][k+59] * tmp[k+59];
                s61 += a[i][k+60] * tmp[k+60];
                s62 += a[i][k+61] * tmp[k+61];
                s63 += a[i][k+62] * tmp[k+62];
                s64 += a[i][k+63] * tmp[k+63];
            }

            c[i][j] = s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9 + s10 + s11 + s12 + s13 + s14 + s15 + s16 +
                      s17 + s18 + s19 + s20 + s21 + s22 + s23 + s24 + s25 + s26 + s27 + s28 + s29 + s30 + s31 + s32 +
                      s33 + s34 + s35 + s36 + s37 + s38 + s39 + s40 + s41 + s42 + s43 + s44 + s45 + s46 + s47 + s48 +
                      s49 + s50 + s51 + s52 + s53 + s54 + s55 + s56 + s57 + s58 + s59 + s60 + s61 + s62 + s63 + s64;
        }
    }

    int EndMs = GetTickCount();
    std::cout << "Time for buffMultiply with M = 64: " << (static_cast<double>(EndMs - StartMs) / 1000)
              << " seconds;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> buffMultiply128(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();

    for (size_t j = 0; j < size; j++)
    {
        std::vector<double> tmp(size);
        for (size_t k = 0; k < size; k++)
        {
            tmp[k] = b[k][j];
        }

        for (size_t i = 0; i < size; i++)
        {
            double s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0, s6 = 0, s7 = 0, s8 = 0;
            double s9 = 0, s10 = 0, s11 = 0, s12 = 0, s13 = 0, s14 = 0, s15 = 0, s16 = 0;
            double s17 = 0, s18 = 0, s19 = 0, s20 = 0, s21 = 0, s22 = 0, s23 = 0, s24 = 0;
            double s25 = 0, s26 = 0, s27 = 0, s28 = 0, s29 = 0, s30 = 0, s31 = 0, s32 = 0;
            double s33 = 0, s34 = 0, s35 = 0, s36 = 0, s37 = 0, s38 = 0, s39 = 0, s40 = 0;
            double s41 = 0, s42 = 0, s43 = 0, s44 = 0, s45 = 0, s46 = 0, s47 = 0, s48 = 0;
            double s49 = 0, s50 = 0, s51 = 0, s52 = 0, s53 = 0, s54 = 0, s55 = 0, s56 = 0;
            double s57 = 0, s58 = 0, s59 = 0, s60 = 0, s61 = 0, s62 = 0, s63 = 0, s64 = 0;
            double s65 = 0, s66 = 0, s67 = 0, s68 = 0, s69 = 0, s70 = 0, s71 = 0, s72 = 0;
            double s73 = 0, s74 = 0, s75 = 0, s76 = 0, s77 = 0, s78 = 0, s79 = 0, s80 = 0;
            double s81 = 0, s82 = 0, s83 = 0, s84 = 0, s85 = 0, s86 = 0, s87 = 0, s88 = 0;
            double s89 = 0, s90 = 0, s91 = 0, s92 = 0, s93 = 0, s94 = 0, s95 = 0, s96 = 0;
            double s97 = 0, s98 = 0, s99 = 0, s100 = 0, s101 = 0, s102 = 0, s103 = 0, s104 = 0;
            double s105 = 0, s106 = 0, s107 = 0, s108 = 0, s109 = 0, s110 = 0, s111 = 0, s112 = 0;
            double s113 = 0, s114 = 0, s115 = 0, s116 = 0, s117 = 0, s118 = 0, s119 = 0, s120 = 0;
            double s121 = 0, s122 = 0, s123 = 0, s124 = 0, s125 = 0, s126 = 0, s127 = 0, s128 = 0;

            for (size_t k = 0; k < size; k += 128)
            {
                s1  += a[i][k]    * tmp[k];
                s2  += a[i][k+1]  * tmp[k+1];
                s3  += a[i][k+2]  * tmp[k+2];
                s4  += a[i][k+3]  * tmp[k+3];
                s5  += a[i][k+4]  * tmp[k+4];
                s6  += a[i][k+5]  * tmp[k+5];
                s7  += a[i][k+6]  * tmp[k+6];
                s8  += a[i][k+7]  * tmp[k+7];
                s9  += a[i][k+8]  * tmp[k+8];
                s10 += a[i][k+9]  * tmp[k+9];
                s11 += a[i][k+10] * tmp[k+10];
                s12 += a[i][k+11] * tmp[k+11];
                s13 += a[i][k+12] * tmp[k+12];
                s14 += a[i][k+13] * tmp[k+13];
                s15 += a[i][k+14] * tmp[k+14];
                s16 += a[i][k+15] * tmp[k+15];
                s17 += a[i][k+16] * tmp[k+16];
                s18 += a[i][k+17] * tmp[k+17];
                s19 += a[i][k+18] * tmp[k+18];
                s20 += a[i][k+19] * tmp[k+19];
                s21 += a[i][k+20] * tmp[k+20];
                s22 += a[i][k+21] * tmp[k+21];
                s23 += a[i][k+22] * tmp[k+22];
                s24 += a[i][k+23] * tmp[k+23];
                s25 += a[i][k+24] * tmp[k+24];
                s26 += a[i][k+25] * tmp[k+25];
                s27 += a[i][k+26] * tmp[k+26];
                s28 += a[i][k+27] * tmp[k+27];
                s29 += a[i][k+28] * tmp[k+28];
                s30 += a[i][k+29] * tmp[k+29];
                s31 += a[i][k+30] * tmp[k+30];
                s32 += a[i][k+31] * tmp[k+31];
                s33 += a[i][k+32] * tmp[k+32];
                s34 += a[i][k+33] * tmp[k+33];
                s35 += a[i][k+34] * tmp[k+34];
                s36 += a[i][k+35] * tmp[k+35];
                s37 += a[i][k+36] * tmp[k+36];
                s38 += a[i][k+37] * tmp[k+37];
                s39 += a[i][k+38] * tmp[k+38];
                s40 += a[i][k+39] * tmp[k+39];
                s41 += a[i][k+40] * tmp[k+40];
                s42 += a[i][k+41] * tmp[k+41];
                s43 += a[i][k+42] * tmp[k+42];
                s44 += a[i][k+43] * tmp[k+43];
                s45 += a[i][k+44] * tmp[k+44];
                s46 += a[i][k+45] * tmp[k+45];
                s47 += a[i][k+46] * tmp[k+46];
                s48 += a[i][k+47] * tmp[k+47];
                s49 += a[i][k+48] * tmp[k+48];
                s50 += a[i][k+49] * tmp[k+49];
                s51 += a[i][k+50] * tmp[k+50];
                s52 += a[i][k+51] * tmp[k+51];
                s53 += a[i][k+52] * tmp[k+52];
                s54 += a[i][k+53] * tmp[k+53];
                s55 += a[i][k+54] * tmp[k+54];
                s56 += a[i][k+55] * tmp[k+55];
                s57 += a[i][k+56] * tmp[k+56];
                s58 += a[i][k+57] * tmp[k+57];
                s59 += a[i][k+58] * tmp[k+58];
                s60 += a[i][k+59] * tmp[k+59];
                s61 += a[i][k+60] * tmp[k+60];
                s62 += a[i][k+61] * tmp[k+61];
                s63 += a[i][k+62] * tmp[k+62];
                s64 += a[i][k+63] * tmp[k+63];
                s65 += a[i][k+64] * tmp[k+64];
                s66 += a[i][k+65] * tmp[k+65];
                s67 += a[i][k+66] * tmp[k+66];
                s68 += a[i][k+67] * tmp[k+67];
                s69 += a[i][k+68] * tmp[k+68];
                s70 += a[i][k+69] * tmp[k+69];
                s71 += a[i][k+70] * tmp[k+70];
                s72 += a[i][k+71] * tmp[k+71];
                s73 += a[i][k+72] * tmp[k+72];
                s74 += a[i][k+73] * tmp[k+73];
                s75 += a[i][k+74] * tmp[k+74];
                s76 += a[i][k+75] * tmp[k+75];
                s77 += a[i][k+76] * tmp[k+76];
                s78 += a[i][k+77] * tmp[k+77];
                s79 += a[i][k+78] * tmp[k+78];
                s80 += a[i][k+79] * tmp[k+79];
                s81 += a[i][k+80] * tmp[k+80];
                s82 += a[i][k+81] * tmp[k+81];
                s83 += a[i][k+82] * tmp[k+82];
                s84 += a[i][k+83] * tmp[k+83];
                s85 += a[i][k+84] * tmp[k+84];
                s86 += a[i][k+85] * tmp[k+85];
                s87 += a[i][k+86] * tmp[k+86];
                s88 += a[i][k+87] * tmp[k+87];
                s89 += a[i][k+88] * tmp[k+88];
                s90 += a[i][k+89] * tmp[k+89];
                s91 += a[i][k+90] * tmp[k+90];
                s92 += a[i][k+91] * tmp[k+91];
                s93 += a[i][k+92] * tmp[k+92];
                s94 += a[i][k+93] * tmp[k+93];
                s95 += a[i][k+94] * tmp[k+94];
                s96 += a[i][k+95] * tmp[k+95];
                s97 += a[i][k+96] * tmp[k+96];
                s98 += a[i][k+97] * tmp[k+97];
                s99 += a[i][k+98] * tmp[k+98];
                s100 += a[i][k+99] * tmp[k+99];
                s101 += a[i][k+100] * tmp[k+100];
                s102 += a[i][k+101] * tmp[k+101];
                s103 += a[i][k+102] * tmp[k+102];
                s104 += a[i][k+103] * tmp[k+103];
                s105 += a[i][k+104] * tmp[k+104];
                s106 += a[i][k+105] * tmp[k+105];
                s107 += a[i][k+106] * tmp[k+106];
                s108 += a[i][k+107] * tmp[k+107];
                s109 += a[i][k+108] * tmp[k+108];
                s110 += a[i][k+109] * tmp[k+109];
                s111 += a[i][k+110] * tmp[k+110];
                s112 += a[i][k+111] * tmp[k+111];
                s113 += a[i][k+112] * tmp[k+112];
                s114 += a[i][k+113] * tmp[k+113];
                s115 += a[i][k+114] * tmp[k+114];
                s116 += a[i][k+115] * tmp[k+115];
                s117 += a[i][k+116] * tmp[k+116];
                s118 += a[i][k+117] * tmp[k+117];
                s119 += a[i][k+118] * tmp[k+118];
                s120 += a[i][k+119] * tmp[k+119];
                s121 += a[i][k+120] * tmp[k+120];
                s122 += a[i][k+121] * tmp[k+121];
                s123 += a[i][k+122] * tmp[k+122];
                s124 += a[i][k+123] * tmp[k+123];
                s125 += a[i][k+124] * tmp[k+124];
                s126 += a[i][k+125] * tmp[k+125];
                s127 += a[i][k+126] * tmp[k+126];
                s128 += a[i][k+127] * tmp[k+127];
            }

            c[i][j] = s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9 + s10 + s11 + s12 + s13 + s14 + s15 + s16 +
                      s17 + s18 + s19 + s20 + s21 + s22 + s23 + s24 + s25 + s26 + s27 + s28 + s29 + s30 + s31 + s32 +
                      s33 + s34 + s35 + s36 + s37 + s38 + s39 + s40 + s41 + s42 + s43 + s44 + s45 + s46 + s47 + s48 +
                      s49 + s50 + s51 + s52 + s53 + s54 + s55 + s56 + s57 + s58 + s59 + s60 + s61 + s62 + s63 + s64 +
                      s65 + s66 + s67 + s68 + s69 + s70 + s71 + s72 + s73 + s74 + s75 + s76 + s77 + s78 + s79 + s80 +
                      s81 + s82 + s83 + s84 + s85 + s86 + s87 + s88 + s89 + s90 + s91 + s92 + s93 + s94 + s95 + s96 +
                      s97 + s98 + s99 + s100 + s101 + s102 + s103 + s104 + s105 + s106 + s107 + s108 + s109 + s110 + s111 + s112 +
                      s113 + s114 + s115 + s116 + s117 + s118 + s119 + s120 + s121 + s122 + s123 + s124 + s125 + s126 + s127 + s128;
        }
    }

    int EndMs = GetTickCount();
    std::cout << "Time for buffMultiply with M = 128: " << (static_cast<double>(EndMs - StartMs) / 1000)
              << " seconds;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> buffMultiply256(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();

    for (size_t j = 0; j < size; j++)
    {
        std::vector<double> tmp(size);
        for (size_t k = 0; k < size; k++)
        {
            tmp[k] = b[k][j];
        }

        for (size_t i = 0; i < size; i++)
        {
            double s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0, s6 = 0, s7 = 0, s8 = 0;
            double s9 = 0, s10 = 0, s11 = 0, s12 = 0, s13 = 0, s14 = 0, s15 = 0, s16 = 0;
            double s17 = 0, s18 = 0, s19 = 0, s20 = 0, s21 = 0, s22 = 0, s23 = 0, s24 = 0;
            double s25 = 0, s26 = 0, s27 = 0, s28 = 0, s29 = 0, s30 = 0, s31 = 0, s32 = 0;
            double s33 = 0, s34 = 0, s35 = 0, s36 = 0, s37 = 0, s38 = 0, s39 = 0, s40 = 0;
            double s41 = 0, s42 = 0, s43 = 0, s44 = 0, s45 = 0, s46 = 0, s47 = 0, s48 = 0;
            double s49 = 0, s50 = 0, s51 = 0, s52 = 0, s53 = 0, s54 = 0, s55 = 0, s56 = 0;
            double s57 = 0, s58 = 0, s59 = 0, s60 = 0, s61 = 0, s62 = 0, s63 = 0, s64 = 0;
            double s65 = 0, s66 = 0, s67 = 0, s68 = 0, s69 = 0, s70 = 0, s71 = 0, s72 = 0;
            double s73 = 0, s74 = 0, s75 = 0, s76 = 0, s77 = 0, s78 = 0, s79 = 0, s80 = 0;
            double s81 = 0, s82 = 0, s83 = 0, s84 = 0, s85 = 0, s86 = 0, s87 = 0, s88 = 0;
            double s89 = 0, s90 = 0, s91 = 0, s92 = 0, s93 = 0, s94 = 0, s95 = 0, s96 = 0;
            double s97 = 0, s98 = 0, s99 = 0, s100 = 0, s101 = 0, s102 = 0, s103 = 0, s104 = 0;
            double s105 = 0, s106 = 0, s107 = 0, s108 = 0, s109 = 0, s110 = 0, s111 = 0, s112 = 0;
            double s113 = 0, s114 = 0, s115 = 0, s116 = 0, s117 = 0, s118 = 0, s119 = 0, s120 = 0;
            double s121 = 0, s122 = 0, s123 = 0, s124 = 0, s125 = 0, s126 = 0, s127 = 0, s128 = 0;
            double s129 = 0, s130 = 0, s131 = 0, s132 = 0, s133 = 0, s134 = 0, s135 = 0, s136 = 0;
            double s137 = 0, s138 = 0, s139 = 0, s140 = 0, s141 = 0, s142 = 0, s143 = 0, s144 = 0;
            double s145 = 0, s146 = 0, s147 = 0, s148 = 0, s149 = 0, s150 = 0, s151 = 0, s152 = 0;
            double s153 = 0, s154 = 0, s155 = 0, s156 = 0, s157 = 0, s158 = 0, s159 = 0, s160 = 0;
            double s161 = 0, s162 = 0, s163 = 0, s164 = 0, s165 = 0, s166 = 0, s167 = 0, s168 = 0;
            double s169 = 0, s170 = 0, s171 = 0, s172 = 0, s173 = 0, s174 = 0, s175 = 0, s176 = 0;
            double s177 = 0, s178 = 0, s179 = 0, s180 = 0, s181 = 0, s182 = 0, s183 = 0, s184 = 0;
            double s185 = 0, s186 = 0, s187 = 0, s188 = 0, s189 = 0, s190 = 0, s191 = 0, s192 = 0;
            double s193 = 0, s194 = 0, s195 = 0, s196 = 0, s197 = 0, s198 = 0, s199 = 0, s200 = 0;
            double s201 = 0, s202 = 0, s203 = 0, s204 = 0, s205 = 0, s206 = 0, s207 = 0, s208 = 0;
            double s209 = 0, s210 = 0, s211 = 0, s212 = 0, s213 = 0, s214 = 0, s215 = 0, s216 = 0;
            double s217 = 0, s218 = 0, s219 = 0, s220 = 0, s221 = 0, s222 = 0, s223 = 0, s224 = 0;
            double s225 = 0, s226 = 0, s227 = 0, s228 = 0, s229 = 0, s230 = 0, s231 = 0, s232 = 0;
            double s233 = 0, s234 = 0, s235 = 0, s236 = 0, s237 = 0, s238 = 0, s239 = 0, s240 = 0;
            double s241 = 0, s242 = 0, s243 = 0, s244 = 0, s245 = 0, s246 = 0, s247 = 0, s248 = 0;
            double s249 = 0, s250 = 0, s251 = 0, s252 = 0, s253 = 0, s254 = 0, s255 = 0, s256 = 0;

            for (size_t k = 0; k < size; k += 256)
            {
                s1   += a[i][k]     * tmp[k];
                s2   += a[i][k+1]   * tmp[k+1];
                s3   += a[i][k+2]   * tmp[k+2];
                s4   += a[i][k+3]   * tmp[k+3];
                s5   += a[i][k+4]   * tmp[k+4];
                s6   += a[i][k+5]   * tmp[k+5];
                s7   += a[i][k+6]   * tmp[k+6];
                s8   += a[i][k+7]   * tmp[k+7];
                s9   += a[i][k+8]   * tmp[k+8];
                s10  += a[i][k+9]   * tmp[k+9];
                s11  += a[i][k+10]  * tmp[k+10];
                s12  += a[i][k+11]  * tmp[k+11];
                s13  += a[i][k+12]  * tmp[k+12];
                s14  += a[i][k+13]  * tmp[k+13];
                s15  += a[i][k+14]  * tmp[k+14];
                s16  += a[i][k+15]  * tmp[k+15];
                s17  += a[i][k+16]  * tmp[k+16];
                s18  += a[i][k+17]  * tmp[k+17];
                s19  += a[i][k+18]  * tmp[k+18];
                s20  += a[i][k+19]  * tmp[k+19];
                s21  += a[i][k+20]  * tmp[k+20];
                s22  += a[i][k+21]  * tmp[k+21];
                s23  += a[i][k+22]  * tmp[k+22];
                s24  += a[i][k+23]  * tmp[k+23];
                s25  += a[i][k+24]  * tmp[k+24];
                s26  += a[i][k+25]  * tmp[k+25];
                s27  += a[i][k+26]  * tmp[k+26];
                s28  += a[i][k+27]  * tmp[k+27];
                s29  += a[i][k+28]  * tmp[k+28];
                s30  += a[i][k+29]  * tmp[k+29];
                s31  += a[i][k+30]  * tmp[k+30];
                s32  += a[i][k+31]  * tmp[k+31];
                s33  += a[i][k+32]  * tmp[k+32];
                s34  += a[i][k+33]  * tmp[k+33];
                s35  += a[i][k+34]  * tmp[k+34];
                s36  += a[i][k+35]  * tmp[k+35];
                s37  += a[i][k+36]  * tmp[k+36];
                s38  += a[i][k+37]  * tmp[k+37];
                s39  += a[i][k+38]  * tmp[k+38];
                s40  += a[i][k+39]  * tmp[k+39];
                s41  += a[i][k+40]  * tmp[k+40];
                s42  += a[i][k+41]  * tmp[k+41];
                s43  += a[i][k+42]  * tmp[k+42];
                s44  += a[i][k+43]  * tmp[k+43];
                s45  += a[i][k+44]  * tmp[k+44];
                s46  += a[i][k+45]  * tmp[k+45];
                s47  += a[i][k+46]  * tmp[k+46];
                s48  += a[i][k+47]  * tmp[k+47];
                s49  += a[i][k+48]  * tmp[k+48];
                s50  += a[i][k+49]  * tmp[k+49];
                s51  += a[i][k+50]  * tmp[k+50];
                s52  += a[i][k+51]  * tmp[k+51];
                s53  += a[i][k+52]  * tmp[k+52];
                s54  += a[i][k+53]  * tmp[k+53];
                s55  += a[i][k+54]  * tmp[k+54];
                s56  += a[i][k+55]  * tmp[k+55];
                s57  += a[i][k+56]  * tmp[k+56];
                s58  += a[i][k+57]  * tmp[k+57];
                s59  += a[i][k+58]  * tmp[k+58];
                s60  += a[i][k+59]  * tmp[k+59];
                s61  += a[i][k+60]  * tmp[k+60];
                s62  += a[i][k+61]  * tmp[k+61];
                s63  += a[i][k+62]  * tmp[k+62];
                s64  += a[i][k+63]  * tmp[k+63];
                s65  += a[i][k+64]  * tmp[k+64];
                s66  += a[i][k+65]  * tmp[k+65];
                s67  += a[i][k+66]  * tmp[k+66];
                s68  += a[i][k+67]  * tmp[k+67];
                s69  += a[i][k+68]  * tmp[k+68];
                s70  += a[i][k+69]  * tmp[k+69];
                s71  += a[i][k+70]  * tmp[k+70];
                s72  += a[i][k+71]  * tmp[k+71];
                s73  += a[i][k+72]  * tmp[k+72];
                s74  += a[i][k+73]  * tmp[k+73];
                s75  += a[i][k+74]  * tmp[k+74];
                s76  += a[i][k+75]  * tmp[k+75];
                s77  += a[i][k+76]  * tmp[k+76];
                s78  += a[i][k+77]  * tmp[k+77];
                s79  += a[i][k+78]  * tmp[k+78];
                s80  += a[i][k+79]  * tmp[k+79];
                s81  += a[i][k+80]  * tmp[k+80];
                s82  += a[i][k+81]  * tmp[k+81];
                s83  += a[i][k+82]  * tmp[k+82];
                s84  += a[i][k+83]  * tmp[k+83];
                s85  += a[i][k+84]  * tmp[k+84];
                s86  += a[i][k+85]  * tmp[k+85];
                s87  += a[i][k+86]  * tmp[k+86];
                s88  += a[i][k+87]  * tmp[k+87];
                s89  += a[i][k+88]  * tmp[k+88];
                s90  += a[i][k+89]  * tmp[k+89];
                s91  += a[i][k+90]  * tmp[k+90];
                s92  += a[i][k+91]  * tmp[k+91];
                s93  += a[i][k+92]  * tmp[k+92];
                s94  += a[i][k+93]  * tmp[k+93];
                s95  += a[i][k+94]  * tmp[k+94];
                s96  += a[i][k+95]  * tmp[k+95];
                s97  += a[i][k+96]  * tmp[k+96];
                s98  += a[i][k+97]  * tmp[k+97];
                s99  += a[i][k+98]  * tmp[k+98];
                s100 += a[i][k+99]  * tmp[k+99];
                s101 += a[i][k+100] * tmp[k+100];
                s102 += a[i][k+101] * tmp[k+101];
                s103 += a[i][k+102] * tmp[k+102];
                s104 += a[i][k+103] * tmp[k+103];
                s105 += a[i][k+104] * tmp[k+104];
                s106 += a[i][k+105] * tmp[k+105];
                s107 += a[i][k+106] * tmp[k+106];
                s108 += a[i][k+107] * tmp[k+107];
                s109 += a[i][k+108] * tmp[k+108];
                s110 += a[i][k+109] * tmp[k+109];
                s111 += a[i][k+110] * tmp[k+110];
                s112 += a[i][k+111] * tmp[k+111];
                s113 += a[i][k+112] * tmp[k+112];
                s114 += a[i][k+113] * tmp[k+113];
                s115 += a[i][k+114] * tmp[k+114];
                s116 += a[i][k+115] * tmp[k+115];
                s117 += a[i][k+116] * tmp[k+116];
                s118 += a[i][k+117] * tmp[k+117];
                s119 += a[i][k+118] * tmp[k+118];
                s120 += a[i][k+119] * tmp[k+119];
                s121 += a[i][k+120] * tmp[k+120];
                s122 += a[i][k+121] * tmp[k+121];
                s123 += a[i][k+122] * tmp[k+122];
                s124 += a[i][k+123] * tmp[k+123];
                s125 += a[i][k+124] * tmp[k+124];
                s126 += a[i][k+125] * tmp[k+125];
                s127 += a[i][k+126] * tmp[k+126];
                s128 += a[i][k+127] * tmp[k+127];
                s129 += a[i][k+128] * tmp[k+128];
                s130 += a[i][k+129] * tmp[k+129];
                s131 += a[i][k+130] * tmp[k+130];
                s132 += a[i][k+131] * tmp[k+131];
                s133 += a[i][k+132] * tmp[k+132];
                s134 += a[i][k+133] * tmp[k+133];
                s135 += a[i][k+134] * tmp[k+134];
                s136 += a[i][k+135] * tmp[k+135];
                s137 += a[i][k+136] * tmp[k+136];
                s138 += a[i][k+137] * tmp[k+137];
                s139 += a[i][k+138] * tmp[k+138];
                s140 += a[i][k+139] * tmp[k+139];
                s141 += a[i][k+140] * tmp[k+140];
                s142 += a[i][k+141] * tmp[k+141];
                s143 += a[i][k+142] * tmp[k+142];
                s144 += a[i][k+143] * tmp[k+143];
                s145 += a[i][k+144] * tmp[k+144];
                s146 += a[i][k+145] * tmp[k+145];
                s147 += a[i][k+146] * tmp[k+146];
                s148 += a[i][k+147] * tmp[k+147];
                s149 += a[i][k+148] * tmp[k+148];
                s150 += a[i][k+149] * tmp[k+149];
                s151 += a[i][k+150] * tmp[k+150];
                s152 += a[i][k+151] * tmp[k+151];
                s153 += a[i][k+152] * tmp[k+152];
                s154 += a[i][k+153] * tmp[k+153];
                s155 += a[i][k+154] * tmp[k+154];
                s156 += a[i][k+155] * tmp[k+155];
                s157 += a[i][k+156] * tmp[k+156];
                s158 += a[i][k+157] * tmp[k+157];
                s159 += a[i][k+158] * tmp[k+158];
                s160 += a[i][k+159] * tmp[k+159];
                s161 += a[i][k+160] * tmp[k+160];
                s162 += a[i][k+161] * tmp[k+161];
                s163 += a[i][k+162] * tmp[k+162];
                s164 += a[i][k+163] * tmp[k+163];
                s165 += a[i][k+164] * tmp[k+164];
                s166 += a[i][k+165] * tmp[k+165];
                s167 += a[i][k+166] * tmp[k+166];
                s168 += a[i][k+167] * tmp[k+167];
                s169 += a[i][k+168] * tmp[k+168];
                s170 += a[i][k+169] * tmp[k+169];
                s171 += a[i][k+170] * tmp[k+170];
                s172 += a[i][k+171] * tmp[k+171];
                s173 += a[i][k+172] * tmp[k+172];
                s174 += a[i][k+173] * tmp[k+173];
                s175 += a[i][k+174] * tmp[k+174];
                s176 += a[i][k+175] * tmp[k+175];
                s177 += a[i][k+176] * tmp[k+176];
                s178 += a[i][k+177] * tmp[k+177];
                s179 += a[i][k+178] * tmp[k+178];
                s180 += a[i][k+179] * tmp[k+179];
                s181 += a[i][k+180] * tmp[k+180];
                s182 += a[i][k+181] * tmp[k+181];
                s183 += a[i][k+182] * tmp[k+182];
                s184 += a[i][k+183] * tmp[k+183];
                s185 += a[i][k+184] * tmp[k+184];
                s186 += a[i][k+185] * tmp[k+185];
                s187 += a[i][k+186] * tmp[k+186];
                s188 += a[i][k+187] * tmp[k+187];
                s189 += a[i][k+188] * tmp[k+188];
                s190 += a[i][k+189] * tmp[k+189];
                s191 += a[i][k+190] * tmp[k+190];
                s192 += a[i][k+191] * tmp[k+191];
                s193 += a[i][k+192] * tmp[k+192];
                s194 += a[i][k+193] * tmp[k+193];
                s195 += a[i][k+194] * tmp[k+194];
                s196 += a[i][k+195] * tmp[k+195];
                s197 += a[i][k+196] * tmp[k+196];
                s198 += a[i][k+197] * tmp[k+197];
                s199 += a[i][k+198] * tmp[k+198];
                s200 += a[i][k+199] * tmp[k+199];
                s201 += a[i][k+200] * tmp[k+200];
                s202 += a[i][k+201] * tmp[k+201];
                s203 += a[i][k+202] * tmp[k+202];
                s204 += a[i][k+203] * tmp[k+203];
                s205 += a[i][k+204] * tmp[k+204];
                s206 += a[i][k+205] * tmp[k+205];
                s207 += a[i][k+206] * tmp[k+206];
                s208 += a[i][k+207] * tmp[k+207];
                s209 += a[i][k+208] * tmp[k+208];
                s210 += a[i][k+209] * tmp[k+209];
                s211 += a[i][k+210] * tmp[k+210];
                s212 += a[i][k+211] * tmp[k+211];
                s213 += a[i][k+212] * tmp[k+212];
                s214 += a[i][k+213] * tmp[k+213];
                s215 += a[i][k+214] * tmp[k+214];
                s216 += a[i][k+215] * tmp[k+215];
                s217 += a[i][k+216] * tmp[k+216];
                s218 += a[i][k+217] * tmp[k+217];
                s219 += a[i][k+218] * tmp[k+218];
                s220 += a[i][k+219] * tmp[k+219];
                s221 += a[i][k+220] * tmp[k+220];
                s222 += a[i][k+221] * tmp[k+221];
                s223 += a[i][k+222] * tmp[k+222];
                s224 += a[i][k+223] * tmp[k+223];
                s225 += a[i][k+224] * tmp[k+224];
                s226 += a[i][k+225] * tmp[k+225];
                s227 += a[i][k+226] * tmp[k+226];
                s228 += a[i][k+227] * tmp[k+227];
                s229 += a[i][k+228] * tmp[k+228];
                s230 += a[i][k+229] * tmp[k+229];
                s231 += a[i][k+230] * tmp[k+230];
                s232 += a[i][k+231] * tmp[k+231];
                s233 += a[i][k+232] * tmp[k+232];
                s234 += a[i][k+233] * tmp[k+233];
                s235 += a[i][k+234] * tmp[k+234];
                s236 += a[i][k+235] * tmp[k+235];
                s237 += a[i][k+236] * tmp[k+236];
                s238 += a[i][k+237] * tmp[k+237];
                s239 += a[i][k+238] * tmp[k+238];
                s240 += a[i][k+239] * tmp[k+239];
                s241 += a[i][k+240] * tmp[k+240];
                s242 += a[i][k+241] * tmp[k+241];
                s243 += a[i][k+242] * tmp[k+242];
                s244 += a[i][k+243] * tmp[k+243];
                s245 += a[i][k+244] * tmp[k+244];
                s246 += a[i][k+245] * tmp[k+245];
                s247 += a[i][k+246] * tmp[k+246];
                s248 += a[i][k+247] * tmp[k+247];
                s249 += a[i][k+248] * tmp[k+248];
                s250 += a[i][k+249] * tmp[k+249];
                s251 += a[i][k+250] * tmp[k+250];
                s252 += a[i][k+251] * tmp[k+251];
                s253 += a[i][k+252] * tmp[k+252];
                s254 += a[i][k+253] * tmp[k+253];
                s255 += a[i][k+254] * tmp[k+254];
                s256 += a[i][k+255] * tmp[k+255];
            }

            c[i][j] = s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9 + s10 + s11 + s12 + s13 + s14 + s15 + s16 +
          s17 + s18 + s19 + s20 + s21 + s22 + s23 + s24 + s25 + s26 + s27 + s28 + s29 + s30 + s31 + s32 +
          s33 + s34 + s35 + s36 + s37 + s38 + s39 + s40 + s41 + s42 + s43 + s44 + s45 + s46 + s47 + s48 +
          s49 + s50 + s51 + s52 + s53 + s54 + s55 + s56 + s57 + s58 + s59 + s60 + s61 + s62 + s63 + s64 +
          s65 + s66 + s67 + s68 + s69 + s70 + s71 + s72 + s73 + s74 + s75 + s76 + s77 + s78 + s79 + s80 +
          s81 + s82 + s83 + s84 + s85 + s86 + s87 + s88 + s89 + s90 + s91 + s92 + s93 + s94 + s95 + s96 +
          s97 + s98 + s99 + s100 + s101 + s102 + s103 + s104 + s105 + s106 + s107 + s108 + s109 + s110 + s111 + s112 +
          s113 + s114 + s115 + s116 + s117 + s118 + s119 + s120 + s121 + s122 + s123 + s124 + s125 + s126 + s127 + s128 +
          s129 + s130 + s131 + s132 + s133 + s134 + s135 + s136 + s137 + s138 + s139 + s140 + s141 + s142 + s143 + s144 +
          s145 + s146 + s147 + s148 + s149 + s150 + s151 + s152 + s153 + s154 + s155 + s156 + s157 + s158 + s159 + s160 +
          s161 + s162 + s163 + s164 + s165 + s166 + s167 + s168 + s169 + s170 + s171 + s172 + s173 + s174 + s175 + s176 +
          s177 + s178 + s179 + s180 + s181 + s182 + s183 + s184 + s185 + s186 + s187 + s188 + s189 + s190 + s191 + s192 +
          s193 + s194 + s195 + s196 + s197 + s198 + s199 + s200 + s201 + s202 + s203 + s204 + s205 + s206 + s207 + s208 +
          s209 + s210 + s211 + s212 + s213 + s214 + s215 + s216 + s217 + s218 + s219 + s220 + s221 + s222 + s223 + s224 +
          s225 + s226 + s227 + s228 + s229 + s230 + s231 + s232 + s233 + s234 + s235 + s236 + s237 + s238 + s239 + s240 +
          s241 + s242 + s243 + s244 + s245 + s246 + s247 + s248 + s249 + s250 + s251 + s252 + s253 + s254 + s255 + s256;
        }
    }

    int EndMs = GetTickCount();
    std::cout << "Time for buffMultiply with M = 256: " << (static_cast<double>(EndMs - StartMs) / 1000)
              << " seconds;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> blockMultiply1(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b, int b_size)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (int bi = 0; bi < size; bi += b_size) {
        for (int bj = 0; bj < size; bj += b_size) {
            for (int bk = 0; bk < size; bk += b_size) {

                int i_end = bi + b_size;
                int j_end = bj + b_size;
                int k_end = bk + b_size;

                for (int i = bi; i < i_end; i++) {
                    for (int j = bj; j < j_end; j++) {
                        double s = 0.0;

                        for (int k = bk; k < k_end; k++) {
                            s += a[i][k] * b[k][j];
                        }
                        c[i][j] += s;
                    }
                }
            }
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for blockMultiply: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   BlockSize: " << b_size << ";   M = 1;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> blockMultiply2(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b, int b_size)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (int bi = 0; bi < size; bi += b_size) {
        for (int bj = 0; bj < size; bj += b_size) {
            for (int bk = 0; bk < size; bk += b_size) {

                int i_end = bi + b_size;
                int j_end = bj + b_size;
                int k_end = bk + b_size;

                for (int i = bi; i < i_end; i++) {
                    for (int j = bj; j < j_end; j++) {
                        double s1 = 0.0;
                        double s2 = 0.0;

                        for (int k = bk; k < k_end; k += 2) {
                            s1 += a[i][k] * b[k][j];
                            s1 += a[i][k+1] * b[k+1][j];
                        }
                        c[i][j] += s1 + s2;
                    }
                }
            }
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for blockMultiply: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   BlockSize: " << b_size << ";   M = 2;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> blockMultiply4(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b, int b_size)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (int bi = 0; bi < size; bi += b_size) {
        for (int bj = 0; bj < size; bj += b_size) {
            for (int bk = 0; bk < size; bk += b_size) {

                int i_end = bi + b_size;
                int j_end = bj + b_size;
                int k_end = bk + b_size;

                for (int i = bi; i < i_end; i++) {
                    for (int j = bj; j < j_end; j++) {
                        double s1 = 0;
                        double s2 = 0;
                        double s3 = 0;
                        double s4 = 0;

                        for (int k = bk; k < k_end; k += 4) {
                            s1 += a[i][k] * b[k][j];
                            s2 += a[i][k+1] * b[k+1][j];
                            s3 += a[i][k+2] * b[k+2][j];
                            s4 += a[i][k+3] * b[k+3][j];
                        }
                        c[i][j] += s1 + s2 + s3 + s4;
                    }
                }
            }
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for blockMultiply: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   BlockSize: " << b_size << ";   M = 4;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> blockMultiply8(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b, int b_size)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (int bi = 0; bi < size; bi += b_size) {
        for (int bj = 0; bj < size; bj += b_size) {
            for (int bk = 0; bk < size; bk += b_size) {

                int i_end = bi + b_size;
                int j_end = bj + b_size;
                int k_end = bk + b_size;

                for (int i = bi; i < i_end; i++) {
                    for (int j = bj; j < j_end; j++) {
                        double s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0, s6 = 0, s7 = 0, s8 = 0;  

                        for (int k = bk; k < k_end; k += 8) {
                            s1 += a[i][k]   * b[k][j];
                            s2 += a[i][k+1] * b[k+1][j];
                            s3 += a[i][k+2] * b[k+2][j];
                            s4 += a[i][k+3] * b[k+3][j];
                            s5 += a[i][k+4] * b[k+4][j];
                            s6 += a[i][k+5] * b[k+5][j];
                            s7 += a[i][k+6] * b[k+6][j];
                            s8 += a[i][k+7] * b[k+7][j];
                        }
                        c[i][j] += s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8;
                    }
                }
            }
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for blockMultiply: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   BlockSize: " << b_size << ";   M = 8;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> blockMultiply16(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b, int b_size)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (int bi = 0; bi < size; bi += b_size) {
        for (int bj = 0; bj < size; bj += b_size) {
            for (int bk = 0; bk < size; bk += b_size) {

                int i_end = bi + b_size;
                int j_end = bj + b_size;
                int k_end = bk + b_size;

                for (int i = bi; i < i_end; i++) {
                    for (int j = bj; j < j_end; j++) {
                        double s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0, s6 = 0, s7 = 0, s8 = 0;
                        double s9 = 0, s10 = 0, s11 = 0, s12 = 0, s13 = 0, s14 = 0, s15 = 0, s16 = 0;

                        for (int k = bk; k < k_end; k += 16) {
                            s1  += a[i][k]    * b[k][j];
                            s2  += a[i][k+1]  * b[k+1][j];
                            s3  += a[i][k+2]  * b[k+2][j];
                            s4  += a[i][k+3]  * b[k+3][j];
                            s5  += a[i][k+4]  * b[k+4][j];
                            s6  += a[i][k+5]  * b[k+5][j];
                            s7  += a[i][k+6]  * b[k+6][j];
                            s8  += a[i][k+7]  * b[k+7][j];
                            s9  += a[i][k+8]  * b[k+8][j];
                            s10 += a[i][k+9]  * b[k+9][j];
                            s11 += a[i][k+10] * b[k+10][j];
                            s12 += a[i][k+11] * b[k+11][j];
                            s13 += a[i][k+12] * b[k+12][j];
                            s14 += a[i][k+13] * b[k+13][j];
                            s15 += a[i][k+14] * b[k+14][j];
                            s16 += a[i][k+15] * b[k+15][j];
                        }
                        c[i][j] += s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9 + s10 + s11 + s12 + s13 + s14 + s15 + s16;
                    }
                }
            }
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for blockMultiply: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   BlockSize: " << b_size << ";   M = 16;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> blockMultiply32(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b, int b_size)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (int bi = 0; bi < size; bi += b_size) {
        for (int bj = 0; bj < size; bj += b_size) {
            for (int bk = 0; bk < size; bk += b_size) {

                int i_end = bi + b_size;
                int j_end = bj + b_size;
                int k_end = bk + b_size;

                for (int i = bi; i < i_end; i++) {
                    for (int j = bj; j < j_end; j++) {
                        double s1=0,s2=0,s3=0,s4=0,s5=0,s6=0,s7=0,s8=0,s9=0,s10=0,s11=0,s12=0,s13=0,s14=0,s15=0,s16=0;
                        double s17=0,s18=0,s19=0,s20=0,s21=0,s22=0,s23=0,s24=0,s25=0,s26=0,s27=0,s28=0,s29=0,s30=0,s31=0,s32=0;

                        for (int k = bk; k < k_end; k+=32) {
                            s1  += a[i][k]    * b[k][j];
                            s2  += a[i][k+1]  * b[k+1][j];
                            s3  += a[i][k+2]  * b[k+2][j];
                            s4  += a[i][k+3]  * b[k+3][j];
                            s5  += a[i][k+4]  * b[k+4][j];
                            s6  += a[i][k+5]  * b[k+5][j];
                            s7  += a[i][k+6]  * b[k+6][j];
                            s8  += a[i][k+7]  * b[k+7][j];
                            s9  += a[i][k+8]  * b[k+8][j];
                            s10 += a[i][k+9]  * b[k+9][j];
                            s11 += a[i][k+10] * b[k+10][j];
                            s12 += a[i][k+11] * b[k+11][j];
                            s13 += a[i][k+12] * b[k+12][j];
                            s14 += a[i][k+13] * b[k+13][j];
                            s15 += a[i][k+14] * b[k+14][j];
                            s16 += a[i][k+15] * b[k+15][j];
                            s17 += a[i][k+16] * b[k+16][j];
                            s18 += a[i][k+17] * b[k+17][j];
                            s19 += a[i][k+18] * b[k+18][j];
                            s20 += a[i][k+19] * b[k+19][j];
                            s21 += a[i][k+20] * b[k+20][j];
                            s22 += a[i][k+21] * b[k+21][j];
                            s23 += a[i][k+22] * b[k+22][j];
                            s24 += a[i][k+23] * b[k+23][j];
                            s25 += a[i][k+24] * b[k+24][j];
                            s26 += a[i][k+25] * b[k+25][j];
                            s27 += a[i][k+26] * b[k+26][j];
                            s28 += a[i][k+27] * b[k+27][j];
                            s29 += a[i][k+28] * b[k+28][j];
                            s30 += a[i][k+29] * b[k+29][j];
                            s31 += a[i][k+30] * b[k+30][j];
                            s32 += a[i][k+31] * b[k+31][j];
                        }
                        c[i][j] += s1+s2+s3+s4+s5+s6+s7+s8+s9+s10+s11+s12+s13+s14+s15+s16+
                                   s17+s18+s19+s20+s21+s22+s23+s24+s25+s26+s27+s28+s29+s30+s31+s32;
                    }
                }
            }
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for blockMultiply: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   BlockSize: " << b_size << ";   M = 32;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> blockMultiply64(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b, int b_size)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (int bi = 0; bi < size; bi += b_size) {
        for (int bj = 0; bj < size; bj += b_size) {
            for (int bk = 0; bk < size; bk += b_size) {

                int i_end = bi + b_size;
                int j_end = bj + b_size;
                int k_end = bk + b_size;

                for (int i = bi; i < i_end; i++) {
                    for (int j = bj; j < j_end; j++) {
                        double s1=0,s2=0,s3=0,s4=0,s5=0,s6=0,s7=0,s8=0,s9=0,s10=0,s11=0,s12=0,s13=0,s14=0,s15=0,s16=0;
                        double s17=0,s18=0,s19=0,s20=0,s21=0,s22=0,s23=0,s24=0,s25=0,s26=0,s27=0,s28=0,s29=0,s30=0,s31=0,s32=0;
                        double s33=0,s34=0,s35=0,s36=0,s37=0,s38=0,s39=0,s40=0,s41=0,s42=0,s43=0,s44=0,s45=0,s46=0,s47=0,s48=0;
                        double s49=0,s50=0,s51=0,s52=0,s53=0,s54=0,s55=0,s56=0,s57=0,s58=0,s59=0,s60=0,s61=0,s62=0,s63=0,s64=0;

                        for (int k = bk; k < k_end; k += 64) {
                            s1  += a[i][k]    * b[k][j];  
                            s2  += a[i][k+1]  * b[k+1][j];
                            s3  += a[i][k+2]  * b[k+2][j]; 
                            s4  += a[i][k+3]  * b[k+3][j];
                            s5  += a[i][k+4]  * b[k+4][j]; 
                            s6  += a[i][k+5]  * b[k+5][j]; 
                            s7  += a[i][k+6]  * b[k+6][j]; 
                            s8  += a[i][k+7]  * b[k+7][j];
                            s9  += a[i][k+8]  * b[k+8][j];
                            s10 += a[i][k+9]  * b[k+9][j]; 
                            s11 += a[i][k+10] * b[k+10][j];
                            s12 += a[i][k+11] * b[k+11][j];
                            s13 += a[i][k+12] * b[k+12][j]; 
                            s14 += a[i][k+13] * b[k+13][j];
                            s15 += a[i][k+14] * b[k+14][j]; 
                            s16 += a[i][k+15] * b[k+15][j];
                            s17 += a[i][k+16] * b[k+16][j]; 
                            s18 += a[i][k+17] * b[k+17][j]; 
                            s19 += a[i][k+18] * b[k+18][j]; 
                            s20 += a[i][k+19] * b[k+19][j]; 
                            s21 += a[i][k+20] * b[k+20][j]; 
                            s22 += a[i][k+21] * b[k+21][j]; 
                            s23 += a[i][k+22] * b[k+22][j]; 
                            s24 += a[i][k+23] * b[k+23][j]; 
                            s25 += a[i][k+24] * b[k+24][j];
                            s26 += a[i][k+25] * b[k+25][j]; 
                            s27 += a[i][k+26] * b[k+26][j]; 
                            s28 += a[i][k+27] * b[k+27][j]; 
                            s29 += a[i][k+28] * b[k+28][j]; 
                            s30 += a[i][k+29] * b[k+29][j]; 
                            s31 += a[i][k+30] * b[k+30][j];
                            s32 += a[i][k+31] * b[k+31][j];
                            s33 += a[i][k+32] * b[k+32][j];
                            s34 += a[i][k+33] * b[k+33][j];
                            s35 += a[i][k+34] * b[k+34][j];
                            s36 += a[i][k+35] * b[k+35][j];
                            s37 += a[i][k+36] * b[k+36][j];
                            s38 += a[i][k+37] * b[k+37][j];
                            s39 += a[i][k+38] * b[k+38][j];
                            s40 += a[i][k+39] * b[k+39][j];
                            s41 += a[i][k+40] * b[k+40][j];
                            s42 += a[i][k+41] * b[k+41][j];
                            s43 += a[i][k+42] * b[k+42][j];
                            s44 += a[i][k+43] * b[k+43][j];
                            s45 += a[i][k+44] * b[k+44][j];
                            s46 += a[i][k+45] * b[k+45][j];
                            s47 += a[i][k+46] * b[k+46][j];
                            s48 += a[i][k+47] * b[k+47][j];
                            s49 += a[i][k+48] * b[k+48][j];
                            s50 += a[i][k+49] * b[k+49][j];
                            s51 += a[i][k+50] * b[k+50][j];
                            s52 += a[i][k+51] * b[k+51][j];
                            s53 += a[i][k+52] * b[k+52][j];
                            s54 += a[i][k+53] * b[k+53][j];
                            s55 += a[i][k+54] * b[k+54][j];
                            s56 += a[i][k+55] * b[k+55][j];
                            s57 += a[i][k+56] * b[k+56][j];
                            s58 += a[i][k+57] * b[k+57][j];
                            s59 += a[i][k+58] * b[k+58][j];
                            s60 += a[i][k+59] * b[k+59][j];
                            s61 += a[i][k+60] * b[k+60][j];
                            s62 += a[i][k+61] * b[k+61][j];
                            s63 += a[i][k+62] * b[k+62][j];
                            s64 += a[i][k+63] * b[k+63][j];
                        }
                        c[i][j] += s1+s2+s3+s4+s5+s6+s7+s8+s9+s10+s11+s12+s13+s14+s15+s16+
                                   s17+s18+s19+s20+s21+s22+s23+s24+s25+s26+s27+s28+s29+s30+s31+s32+
                                   s33+s34+s35+s36+s37+s38+s39+s40+s41+s42+s43+s44+s45+s46+s47+s48+
                                   s49+s50+s51+s52+s53+s54+s55+s56+s57+s58+s59+s60+s61+s62+s63+s64;
                    }
                }
            }
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for blockMultiply: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   BlockSize: " << b_size << ";   M = 64;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> blockMultiply128(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b, int b_size)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (int bi = 0; bi < size; bi += b_size) {
        for (int bj = 0; bj < size; bj += b_size) {
            for (int bk = 0; bk < size; bk += b_size) {

                int i_end = bi + b_size;
                int j_end = bj + b_size;
                int k_end = bk + b_size;

                for (int i = bi; i < i_end; i++) {
                    for (int j = bj; j < j_end; j++) {
                        double s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0, s6 = 0, s7 = 0, s8 = 0;
                        double s9 = 0, s10 = 0, s11 = 0, s12 = 0, s13 = 0, s14 = 0, s15 = 0, s16 = 0;
                        double s17 = 0, s18 = 0, s19 = 0, s20 = 0, s21 = 0, s22 = 0, s23 = 0, s24 = 0;
                        double s25 = 0, s26 = 0, s27 = 0, s28 = 0, s29 = 0, s30 = 0, s31 = 0, s32 = 0;
                        double s33 = 0, s34 = 0, s35 = 0, s36 = 0, s37 = 0, s38 = 0, s39 = 0, s40 = 0;
                        double s41 = 0, s42 = 0, s43 = 0, s44 = 0, s45 = 0, s46 = 0, s47 = 0, s48 = 0;
                        double s49 = 0, s50 = 0, s51 = 0, s52 = 0, s53 = 0, s54 = 0, s55 = 0, s56 = 0;
                        double s57 = 0, s58 = 0, s59 = 0, s60 = 0, s61 = 0, s62 = 0, s63 = 0, s64 = 0;
                        double s65 = 0, s66 = 0, s67 = 0, s68 = 0, s69 = 0, s70 = 0, s71 = 0, s72 = 0;
                        double s73 = 0, s74 = 0, s75 = 0, s76 = 0, s77 = 0, s78 = 0, s79 = 0, s80 = 0;
                        double s81 = 0, s82 = 0, s83 = 0, s84 = 0, s85 = 0, s86 = 0, s87 = 0, s88 = 0;
                        double s89 = 0, s90 = 0, s91 = 0, s92 = 0, s93 = 0, s94 = 0, s95 = 0, s96 = 0;
                        double s97 = 0, s98 = 0, s99 = 0, s100 = 0, s101 = 0, s102 = 0, s103 = 0, s104 = 0;
                        double s105 = 0, s106 = 0, s107 = 0, s108 = 0, s109 = 0, s110 = 0, s111 = 0, s112 = 0;
                        double s113 = 0, s114 = 0, s115 = 0, s116 = 0, s117 = 0, s118 = 0, s119 = 0, s120 = 0;
                        double s121 = 0, s122 = 0, s123 = 0, s124 = 0, s125 = 0, s126 = 0, s127 = 0, s128 = 0;

                        for (int k = bk; k < k_end; k += 128) {
                            s1   += a[i][k]    * b[k][j];
                            s2   += a[i][k+1]  * b[k+1][j];
                            s3   += a[i][k+2]  * b[k+2][j];
                            s4   += a[i][k+3]  * b[k+3][j];
                            s5   += a[i][k+4]  * b[k+4][j];
                            s6   += a[i][k+5]  * b[k+5][j];
                            s7   += a[i][k+6]  * b[k+6][j];
                            s8   += a[i][k+7]  * b[k+7][j];
                            s9   += a[i][k+8]  * b[k+8][j];
                            s10  += a[i][k+9]  * b[k+9][j];
                            s11  += a[i][k+10] * b[k+10][j];
                            s12  += a[i][k+11] * b[k+11][j];
                            s13  += a[i][k+12] * b[k+12][j];
                            s14  += a[i][k+13] * b[k+13][j];
                            s15  += a[i][k+14] * b[k+14][j];
                            s16  += a[i][k+15] * b[k+15][j];
                            s17  += a[i][k+16] * b[k+16][j];
                            s18  += a[i][k+17] * b[k+17][j];
                            s19  += a[i][k+18] * b[k+18][j];
                            s20  += a[i][k+19] * b[k+19][j];
                            s21  += a[i][k+20] * b[k+20][j];
                            s22  += a[i][k+21] * b[k+21][j];
                            s23  += a[i][k+22] * b[k+22][j];
                            s24  += a[i][k+23] * b[k+23][j];
                            s25  += a[i][k+24] * b[k+24][j];
                            s26  += a[i][k+25] * b[k+25][j];
                            s27  += a[i][k+26] * b[k+26][j];
                            s28  += a[i][k+27] * b[k+27][j];
                            s29  += a[i][k+28] * b[k+28][j];
                            s30  += a[i][k+29] * b[k+29][j];
                            s31  += a[i][k+30] * b[k+30][j];
                            s32  += a[i][k+31] * b[k+31][j];
                            s33  += a[i][k+32] * b[k+32][j];
                            s34  += a[i][k+33] * b[k+33][j];
                            s35  += a[i][k+34] * b[k+34][j];
                            s36  += a[i][k+35] * b[k+35][j];
                            s37  += a[i][k+36] * b[k+36][j];
                            s38  += a[i][k+37] * b[k+37][j];
                            s39  += a[i][k+38] * b[k+38][j];
                            s40  += a[i][k+39] * b[k+39][j];
                            s41  += a[i][k+40] * b[k+40][j];
                            s42  += a[i][k+41] * b[k+41][j];
                            s43  += a[i][k+42] * b[k+42][j];
                            s44  += a[i][k+43] * b[k+43][j];
                            s45  += a[i][k+44] * b[k+44][j];
                            s46  += a[i][k+45] * b[k+45][j];
                            s47  += a[i][k+46] * b[k+46][j];
                            s48  += a[i][k+47] * b[k+47][j];
                            s49  += a[i][k+48] * b[k+48][j];
                            s50  += a[i][k+49] * b[k+49][j];
                            s51  += a[i][k+50] * b[k+50][j];
                            s52  += a[i][k+51] * b[k+51][j];
                            s53  += a[i][k+52] * b[k+52][j];
                            s54  += a[i][k+53] * b[k+53][j];
                            s55  += a[i][k+54] * b[k+54][j];
                            s56  += a[i][k+55] * b[k+55][j];
                            s57  += a[i][k+56] * b[k+56][j];
                            s58  += a[i][k+57] * b[k+57][j];
                            s59  += a[i][k+58] * b[k+58][j];
                            s60  += a[i][k+59] * b[k+59][j];
                            s61  += a[i][k+60] * b[k+60][j];
                            s62  += a[i][k+61] * b[k+61][j];
                            s63  += a[i][k+62] * b[k+62][j];
                            s64  += a[i][k+63] * b[k+63][j];
                            s65  += a[i][k+64] * b[k+64][j];
                            s66  += a[i][k+65] * b[k+65][j];
                            s67  += a[i][k+66] * b[k+66][j];
                            s68  += a[i][k+67] * b[k+67][j];
                            s69  += a[i][k+68] * b[k+68][j];
                            s70  += a[i][k+69] * b[k+69][j];
                            s71  += a[i][k+70] * b[k+70][j];
                            s72  += a[i][k+71] * b[k+71][j];
                            s73  += a[i][k+72] * b[k+72][j];
                            s74  += a[i][k+73] * b[k+73][j];
                            s75  += a[i][k+74] * b[k+74][j];
                            s76  += a[i][k+75] * b[k+75][j];
                            s77  += a[i][k+76] * b[k+76][j];
                            s78  += a[i][k+77] * b[k+77][j];
                            s79  += a[i][k+78] * b[k+78][j];
                            s80  += a[i][k+79] * b[k+79][j];
                            s81  += a[i][k+80] * b[k+80][j];
                            s82  += a[i][k+81] * b[k+81][j];
                            s83  += a[i][k+82] * b[k+82][j];
                            s84  += a[i][k+83] * b[k+83][j];
                            s85  += a[i][k+84] * b[k+84][j];
                            s86  += a[i][k+85] * b[k+85][j];
                            s87  += a[i][k+86] * b[k+86][j];
                            s88  += a[i][k+87] * b[k+87][j];
                            s89  += a[i][k+88] * b[k+88][j];
                            s90  += a[i][k+89] * b[k+89][j];
                            s91  += a[i][k+90] * b[k+90][j];
                            s92  += a[i][k+91] * b[k+91][j];
                            s93  += a[i][k+92] * b[k+92][j];
                            s94  += a[i][k+93] * b[k+93][j];
                            s95  += a[i][k+94] * b[k+94][j];
                            s96  += a[i][k+95] * b[k+95][j];
                            s97  += a[i][k+96] * b[k+96][j];
                            s98  += a[i][k+97] * b[k+97][j];
                            s99  += a[i][k+98] * b[k+98][j];
                            s100 += a[i][k+99] * b[k+99][j];
                            s101 += a[i][k+100] * b[k+100][j];
                            s102 += a[i][k+101] * b[k+101][j];
                            s103 += a[i][k+102] * b[k+102][j];
                            s104 += a[i][k+103] * b[k+103][j];
                            s105 += a[i][k+104] * b[k+104][j];
                            s106 += a[i][k+105] * b[k+105][j];
                            s107 += a[i][k+106] * b[k+106][j];
                            s108 += a[i][k+107] * b[k+107][j];
                            s109 += a[i][k+108] * b[k+108][j];
                            s110 += a[i][k+109] * b[k+109][j];
                            s111 += a[i][k+110] * b[k+110][j];
                            s112 += a[i][k+111] * b[k+111][j];
                            s113 += a[i][k+112] * b[k+112][j];
                            s114 += a[i][k+113] * b[k+113][j];
                            s115 += a[i][k+114] * b[k+114][j];
                            s116 += a[i][k+115] * b[k+115][j];
                            s117 += a[i][k+116] * b[k+116][j];
                            s118 += a[i][k+117] * b[k+117][j];
                            s119 += a[i][k+118] * b[k+118][j];
                            s120 += a[i][k+119] * b[k+119][j];
                            s121 += a[i][k+120] * b[k+120][j];
                            s122 += a[i][k+121] * b[k+121][j];
                            s123 += a[i][k+122] * b[k+122][j];
                            s124 += a[i][k+123] * b[k+123][j];
                            s125 += a[i][k+124] * b[k+124][j];
                            s126 += a[i][k+125] * b[k+125][j];
                            s127 += a[i][k+126] * b[k+126][j];
                            s128 += a[i][k+127] * b[k+127][j];
                        }
                        c[i][j] += s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9 + s10 + s11 + s12 + s13 + s14 + s15 + s16 +
                                   s17 + s18 + s19 + s20 + s21 + s22 + s23 + s24 + s25 + s26 + s27 + s28 + s29 + s30 + s31 + s32 +
                                   s33 + s34 + s35 + s36 + s37 + s38 + s39 + s40 + s41 + s42 + s43 + s44 + s45 + s46 + s47 + s48 +
                                   s49 + s50 + s51 + s52 + s53 + s54 + s55 + s56 + s57 + s58 + s59 + s60 + s61 + s62 + s63 + s64 +
                                   s65 + s66 + s67 + s68 + s69 + s70 + s71 + s72 + s73 + s74 + s75 + s76 + s77 + s78 + s79 + s80 +
                                   s81 + s82 + s83 + s84 + s85 + s86 + s87 + s88 + s89 + s90 + s91 + s92 + s93 + s94 + s95 + s96 +
                                   s97 + s98 + s99 + s100 + s101 + s102 + s103 + s104 + s105 + s106 + s107 + s108 + s109 + s110 + s111 + s112 +
                                   s113 + s114 + s115 + s116 + s117 + s118 + s119 + s120 + s121 + s122 + s123 + s124 + s125 + s126 + s127 + s128;
                    }
                }
            }
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for blockMultiply: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   BlockSize: " << b_size << ";   M = 128;   Size: " << size << "x" << size << "\n\n";
    return c;
}

std::vector<std::vector<double>> blockMultiply256(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b, int b_size)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    int StartMs = GetTickCount();
    for (int bi = 0; bi < size; bi += b_size) {
        for (int bj = 0; bj < size; bj += b_size) {
            for (int bk = 0; bk < size; bk += b_size) {

                int i_end = bi + b_size;
                int j_end = bj + b_size;
                int k_end = bk + b_size;

                for (int i = bi; i < i_end; i++) {
                    for (int j = bj; j < j_end; j++) {
                        double s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0, s6 = 0, s7 = 0, s8 = 0;
                        double s9 = 0, s10 = 0, s11 = 0, s12 = 0, s13 = 0, s14 = 0, s15 = 0, s16 = 0;
                        double s17 = 0, s18 = 0, s19 = 0, s20 = 0, s21 = 0, s22 = 0, s23 = 0, s24 = 0;
                        double s25 = 0, s26 = 0, s27 = 0, s28 = 0, s29 = 0, s30 = 0, s31 = 0, s32 = 0;
                        double s33 = 0, s34 = 0, s35 = 0, s36 = 0, s37 = 0, s38 = 0, s39 = 0, s40 = 0;
                        double s41 = 0, s42 = 0, s43 = 0, s44 = 0, s45 = 0, s46 = 0, s47 = 0, s48 = 0;
                        double s49 = 0, s50 = 0, s51 = 0, s52 = 0, s53 = 0, s54 = 0, s55 = 0, s56 = 0;
                        double s57 = 0, s58 = 0, s59 = 0, s60 = 0, s61 = 0, s62 = 0, s63 = 0, s64 = 0;
                        double s65 = 0, s66 = 0, s67 = 0, s68 = 0, s69 = 0, s70 = 0, s71 = 0, s72 = 0;
                        double s73 = 0, s74 = 0, s75 = 0, s76 = 0, s77 = 0, s78 = 0, s79 = 0, s80 = 0;
                        double s81 = 0, s82 = 0, s83 = 0, s84 = 0, s85 = 0, s86 = 0, s87 = 0, s88 = 0;
                        double s89 = 0, s90 = 0, s91 = 0, s92 = 0, s93 = 0, s94 = 0, s95 = 0, s96 = 0;
                        double s97 = 0, s98 = 0, s99 = 0, s100 = 0, s101 = 0, s102 = 0, s103 = 0, s104 = 0;
                        double s105 = 0, s106 = 0, s107 = 0, s108 = 0, s109 = 0, s110 = 0, s111 = 0, s112 = 0;
                        double s113 = 0, s114 = 0, s115 = 0, s116 = 0, s117 = 0, s118 = 0, s119 = 0, s120 = 0;
                        double s121 = 0, s122 = 0, s123 = 0, s124 = 0, s125 = 0, s126 = 0, s127 = 0, s128 = 0;
                        double s129 = 0, s130 = 0, s131 = 0, s132 = 0, s133 = 0, s134 = 0, s135 = 0, s136 = 0;
                        double s137 = 0, s138 = 0, s139 = 0, s140 = 0, s141 = 0, s142 = 0, s143 = 0, s144 = 0;
                        double s145 = 0, s146 = 0, s147 = 0, s148 = 0, s149 = 0, s150 = 0, s151 = 0, s152 = 0;
                        double s153 = 0, s154 = 0, s155 = 0, s156 = 0, s157 = 0, s158 = 0, s159 = 0, s160 = 0;
                        double s161 = 0, s162 = 0, s163 = 0, s164 = 0, s165 = 0, s166 = 0, s167 = 0, s168 = 0;
                        double s169 = 0, s170 = 0, s171 = 0, s172 = 0, s173 = 0, s174 = 0, s175 = 0, s176 = 0;
                        double s177 = 0, s178 = 0, s179 = 0, s180 = 0, s181 = 0, s182 = 0, s183 = 0, s184 = 0;
                        double s185 = 0, s186 = 0, s187 = 0, s188 = 0, s189 = 0, s190 = 0, s191 = 0, s192 = 0;
                        double s193 = 0, s194 = 0, s195 = 0, s196 = 0, s197 = 0, s198 = 0, s199 = 0, s200 = 0;
                        double s201 = 0, s202 = 0, s203 = 0, s204 = 0, s205 = 0, s206 = 0, s207 = 0, s208 = 0;
                        double s209 = 0, s210 = 0, s211 = 0, s212 = 0, s213 = 0, s214 = 0, s215 = 0, s216 = 0;
                        double s217 = 0, s218 = 0, s219 = 0, s220 = 0, s221 = 0, s222 = 0, s223 = 0, s224 = 0;
                        double s225 = 0, s226 = 0, s227 = 0, s228 = 0, s229 = 0, s230 = 0, s231 = 0, s232 = 0;
                        double s233 = 0, s234 = 0, s235 = 0, s236 = 0, s237 = 0, s238 = 0, s239 = 0, s240 = 0;
                        double s241 = 0, s242 = 0, s243 = 0, s244 = 0, s245 = 0, s246 = 0, s247 = 0, s248 = 0;
                        double s249 = 0, s250 = 0, s251 = 0, s252 = 0, s253 = 0, s254 = 0, s255 = 0, s256 = 0;

                        for (int k = bk; k < k_end; k += 256) {
                            s1   += a[i][k]    * b[k][j];
                            s2   += a[i][k+1]  * b[k+1][j];
                            s3   += a[i][k+2]  * b[k+2][j];
                            s4   += a[i][k+3]  * b[k+3][j];
                            s5   += a[i][k+4]  * b[k+4][j];
                            s6   += a[i][k+5]  * b[k+5][j];
                            s7   += a[i][k+6]  * b[k+6][j];
                            s8   += a[i][k+7]  * b[k+7][j];
                            s9   += a[i][k+8]  * b[k+8][j];
                            s10  += a[i][k+9]  * b[k+9][j];
                            s11  += a[i][k+10] * b[k+10][j];
                            s12  += a[i][k+11] * b[k+11][j];
                            s13  += a[i][k+12] * b[k+12][j];
                            s14  += a[i][k+13] * b[k+13][j];
                            s15  += a[i][k+14] * b[k+14][j];
                            s16  += a[i][k+15] * b[k+15][j];
                            s17  += a[i][k+16] * b[k+16][j];
                            s18  += a[i][k+17] * b[k+17][j];
                            s19  += a[i][k+18] * b[k+18][j];
                            s20  += a[i][k+19] * b[k+19][j];
                            s21  += a[i][k+20] * b[k+20][j];
                            s22  += a[i][k+21] * b[k+21][j];
                            s23  += a[i][k+22] * b[k+22][j];
                            s24  += a[i][k+23] * b[k+23][j];
                            s25  += a[i][k+24] * b[k+24][j];
                            s26  += a[i][k+25] * b[k+25][j];
                            s27  += a[i][k+26] * b[k+26][j];
                            s28  += a[i][k+27] * b[k+27][j];
                            s29  += a[i][k+28] * b[k+28][j];
                            s30  += a[i][k+29] * b[k+29][j];
                            s31  += a[i][k+30] * b[k+30][j];
                            s32  += a[i][k+31] * b[k+31][j];
                            s33  += a[i][k+32] * b[k+32][j];
                            s34  += a[i][k+33] * b[k+33][j];
                            s35  += a[i][k+34] * b[k+34][j];
                            s36  += a[i][k+35] * b[k+35][j];
                            s37  += a[i][k+36] * b[k+36][j];
                            s38  += a[i][k+37] * b[k+37][j];
                            s39  += a[i][k+38] * b[k+38][j];
                            s40  += a[i][k+39] * b[k+39][j];
                            s41  += a[i][k+40] * b[k+40][j];
                            s42  += a[i][k+41] * b[k+41][j];
                            s43  += a[i][k+42] * b[k+42][j];
                            s44  += a[i][k+43] * b[k+43][j];
                            s45  += a[i][k+44] * b[k+44][j];
                            s46  += a[i][k+45] * b[k+45][j];
                            s47  += a[i][k+46] * b[k+46][j];
                            s48  += a[i][k+47] * b[k+47][j];
                            s49  += a[i][k+48] * b[k+48][j];
                            s50  += a[i][k+49] * b[k+49][j];
                            s51  += a[i][k+50] * b[k+50][j];
                            s52  += a[i][k+51] * b[k+51][j];
                            s53  += a[i][k+52] * b[k+52][j];
                            s54  += a[i][k+53] * b[k+53][j];
                            s55  += a[i][k+54] * b[k+54][j];
                            s56  += a[i][k+55] * b[k+55][j];
                            s57  += a[i][k+56] * b[k+56][j];
                            s58  += a[i][k+57] * b[k+57][j];
                            s59  += a[i][k+58] * b[k+58][j];
                            s60  += a[i][k+59] * b[k+59][j];
                            s61  += a[i][k+60] * b[k+60][j];
                            s62  += a[i][k+61] * b[k+61][j];
                            s63  += a[i][k+62] * b[k+62][j];
                            s64  += a[i][k+63] * b[k+63][j];
                            s65  += a[i][k+64] * b[k+64][j];
                            s66  += a[i][k+65] * b[k+65][j];
                            s67  += a[i][k+66] * b[k+66][j];
                            s68  += a[i][k+67] * b[k+67][j];
                            s69  += a[i][k+68] * b[k+68][j];
                            s70  += a[i][k+69] * b[k+69][j];
                            s71  += a[i][k+70] * b[k+70][j];
                            s72  += a[i][k+71] * b[k+71][j];
                            s73  += a[i][k+72] * b[k+72][j];
                            s74  += a[i][k+73] * b[k+73][j];
                            s75  += a[i][k+74] * b[k+74][j];
                            s76  += a[i][k+75] * b[k+75][j];
                            s77  += a[i][k+76] * b[k+76][j];
                            s78  += a[i][k+77] * b[k+77][j];
                            s79  += a[i][k+78] * b[k+78][j];
                            s80  += a[i][k+79] * b[k+79][j];
                            s81  += a[i][k+80] * b[k+80][j];
                            s82  += a[i][k+81] * b[k+81][j];
                            s83  += a[i][k+82] * b[k+82][j];
                            s84  += a[i][k+83] * b[k+83][j];
                            s85  += a[i][k+84] * b[k+84][j];
                            s86  += a[i][k+85] * b[k+85][j];
                            s87  += a[i][k+86] * b[k+86][j];
                            s88  += a[i][k+87] * b[k+87][j];
                            s89  += a[i][k+88] * b[k+88][j];
                            s90  += a[i][k+89] * b[k+89][j];
                            s91  += a[i][k+90] * b[k+90][j];
                            s92  += a[i][k+91] * b[k+91][j];
                            s93  += a[i][k+92] * b[k+92][j];
                            s94  += a[i][k+93] * b[k+93][j];
                            s95  += a[i][k+94] * b[k+94][j];
                            s96  += a[i][k+95] * b[k+95][j];
                            s97  += a[i][k+96] * b[k+96][j];
                            s98  += a[i][k+97] * b[k+97][j];
                            s99  += a[i][k+98] * b[k+98][j];
                            s100 += a[i][k+99] * b[k+99][j];
                            s101 += a[i][k+100] * b[k+100][j];
                            s102 += a[i][k+101] * b[k+101][j];
                            s103 += a[i][k+102] * b[k+102][j];
                            s104 += a[i][k+103] * b[k+103][j];
                            s105 += a[i][k+104] * b[k+104][j];
                            s106 += a[i][k+105] * b[k+105][j];
                            s107 += a[i][k+106] * b[k+106][j];
                            s108 += a[i][k+107] * b[k+107][j];
                            s109 += a[i][k+108] * b[k+108][j];
                            s110 += a[i][k+109] * b[k+109][j];
                            s111 += a[i][k+110] * b[k+110][j];
                            s112 += a[i][k+111] * b[k+111][j];
                            s113 += a[i][k+112] * b[k+112][j];
                            s114 += a[i][k+113] * b[k+113][j];
                            s115 += a[i][k+114] * b[k+114][j];
                            s116 += a[i][k+115] * b[k+115][j];
                            s117 += a[i][k+116] * b[k+116][j];
                            s118 += a[i][k+117] * b[k+117][j];
                            s119 += a[i][k+118] * b[k+118][j];
                            s120 += a[i][k+119] * b[k+119][j];
                            s121 += a[i][k+120] * b[k+120][j];
                            s122 += a[i][k+121] * b[k+121][j];
                            s123 += a[i][k+122] * b[k+122][j];
                            s124 += a[i][k+123] * b[k+123][j];
                            s125 += a[i][k+124] * b[k+124][j];
                            s126 += a[i][k+125] * b[k+125][j];
                            s127 += a[i][k+126] * b[k+126][j];
                            s128 += a[i][k+127] * b[k+127][j];
                            s129 += a[i][k+128] * b[k+128][j];
                            s130 += a[i][k+129] * b[k+129][j];
                            s131 += a[i][k+130] * b[k+130][j];
                            s132 += a[i][k+131] * b[k+131][j];
                            s133 += a[i][k+132] * b[k+132][j];
                            s134 += a[i][k+133] * b[k+133][j];
                            s135 += a[i][k+134] * b[k+134][j];
                            s136 += a[i][k+135] * b[k+135][j];
                            s137 += a[i][k+136] * b[k+136][j];
                            s138 += a[i][k+137] * b[k+137][j];
                            s139 += a[i][k+138] * b[k+138][j];
                            s140 += a[i][k+139] * b[k+139][j];
                            s141 += a[i][k+140] * b[k+140][j];
                            s142 += a[i][k+141] * b[k+141][j];
                            s143 += a[i][k+142] * b[k+142][j];
                            s144 += a[i][k+143] * b[k+143][j];
                            s145 += a[i][k+144] * b[k+144][j];
                            s146 += a[i][k+145] * b[k+145][j];
                            s147 += a[i][k+146] * b[k+146][j];
                            s148 += a[i][k+147] * b[k+147][j];
                            s149 += a[i][k+148] * b[k+148][j];
                            s150 += a[i][k+149] * b[k+149][j];
                            s151 += a[i][k+150] * b[k+150][j];
                            s152 += a[i][k+151] * b[k+151][j];
                            s153 += a[i][k+152] * b[k+152][j];
                            s154 += a[i][k+153] * b[k+153][j];
                            s155 += a[i][k+154] * b[k+154][j];
                            s156 += a[i][k+155] * b[k+155][j];
                            s157 += a[i][k+156] * b[k+156][j];
                            s158 += a[i][k+157] * b[k+157][j];
                            s159 += a[i][k+158] * b[k+158][j];
                            s160 += a[i][k+159] * b[k+159][j];
                            s161 += a[i][k+160] * b[k+160][j];
                            s162 += a[i][k+161] * b[k+161][j];
                            s163 += a[i][k+162] * b[k+162][j];
                            s164 += a[i][k+163] * b[k+163][j];
                            s165 += a[i][k+164] * b[k+164][j];
                            s166 += a[i][k+165] * b[k+165][j];
                            s167 += a[i][k+166] * b[k+166][j];
                            s168 += a[i][k+167] * b[k+167][j];
                            s169 += a[i][k+168] * b[k+168][j];
                            s170 += a[i][k+169] * b[k+169][j];
                            s171 += a[i][k+170] * b[k+170][j];
                            s172 += a[i][k+171] * b[k+171][j];
                            s173 += a[i][k+172] * b[k+172][j];
                            s174 += a[i][k+173] * b[k+173][j];
                            s175 += a[i][k+174] * b[k+174][j];
                            s176 += a[i][k+175] * b[k+175][j];
                            s177 += a[i][k+176] * b[k+176][j];
                            s178 += a[i][k+177] * b[k+177][j];
                            s179 += a[i][k+178] * b[k+178][j];
                            s180 += a[i][k+179] * b[k+179][j];
                            s181 += a[i][k+180] * b[k+180][j];
                            s182 += a[i][k+181] * b[k+181][j];
                            s183 += a[i][k+182] * b[k+182][j];
                            s184 += a[i][k+183] * b[k+183][j];
                            s185 += a[i][k+184] * b[k+184][j];
                            s186 += a[i][k+185] * b[k+185][j];
                            s187 += a[i][k+186] * b[k+186][j];
                            s188 += a[i][k+187] * b[k+187][j];
                            s189 += a[i][k+188] * b[k+188][j];
                            s190 += a[i][k+189] * b[k+189][j];
                            s191 += a[i][k+190] * b[k+190][j];
                            s192 += a[i][k+191] * b[k+191][j];
                            s193 += a[i][k+192] * b[k+192][j];
                            s194 += a[i][k+193] * b[k+193][j];
                            s195 += a[i][k+194] * b[k+194][j];
                            s196 += a[i][k+195] * b[k+195][j];
                            s197 += a[i][k+196] * b[k+196][j];
                            s198 += a[i][k+197] * b[k+197][j];
                            s199 += a[i][k+198] * b[k+198][j];
                            s200 += a[i][k+199] * b[k+199][j];
                            s201 += a[i][k+200] * b[k+200][j];
                            s202 += a[i][k+201] * b[k+201][j];
                            s203 += a[i][k+202] * b[k+202][j];
                            s204 += a[i][k+203] * b[k+203][j];
                            s205 += a[i][k+204] * b[k+204][j];
                            s206 += a[i][k+205] * b[k+205][j];
                            s207 += a[i][k+206] * b[k+206][j];
                            s208 += a[i][k+207] * b[k+207][j];
                            s209 += a[i][k+208] * b[k+208][j];
                            s210 += a[i][k+209] * b[k+209][j];
                            s211 += a[i][k+210] * b[k+210][j];
                            s212 += a[i][k+211] * b[k+211][j];
                            s213 += a[i][k+212] * b[k+212][j];
                            s214 += a[i][k+213] * b[k+213][j];
                            s215 += a[i][k+214] * b[k+214][j];
                            s216 += a[i][k+215] * b[k+215][j];
                            s217 += a[i][k+216] * b[k+216][j];
                            s218 += a[i][k+217] * b[k+217][j];
                            s219 += a[i][k+218] * b[k+218][j];
                            s220 += a[i][k+219] * b[k+219][j];
                            s221 += a[i][k+220] * b[k+220][j];
                            s222 += a[i][k+221] * b[k+221][j];
                            s223 += a[i][k+222] * b[k+222][j];
                            s224 += a[i][k+223] * b[k+223][j];
                            s225 += a[i][k+224] * b[k+224][j];
                            s226 += a[i][k+225] * b[k+225][j];
                            s227 += a[i][k+226] * b[k+226][j];
                            s228 += a[i][k+227] * b[k+227][j];
                            s229 += a[i][k+228] * b[k+228][j];
                            s230 += a[i][k+229] * b[k+229][j];
                            s231 += a[i][k+230] * b[k+230][j];
                            s232 += a[i][k+231] * b[k+231][j];
                            s233 += a[i][k+232] * b[k+232][j];
                            s234 += a[i][k+233] * b[k+233][j];
                            s235 += a[i][k+234] * b[k+234][j];
                            s236 += a[i][k+235] * b[k+235][j];
                            s237 += a[i][k+236] * b[k+236][j];
                            s238 += a[i][k+237] * b[k+237][j];
                            s239 += a[i][k+238] * b[k+238][j];
                            s240 += a[i][k+239] * b[k+239][j];
                            s241 += a[i][k+240] * b[k+240][j];
                            s242 += a[i][k+241] * b[k+241][j];
                            s243 += a[i][k+242] * b[k+242][j];
                            s244 += a[i][k+243] * b[k+243][j];
                            s245 += a[i][k+244] * b[k+244][j];
                            s246 += a[i][k+245] * b[k+245][j];
                            s247 += a[i][k+246] * b[k+246][j];
                            s248 += a[i][k+247] * b[k+247][j];
                            s249 += a[i][k+248] * b[k+248][j];
                            s250 += a[i][k+249] * b[k+249][j];
                            s251 += a[i][k+250] * b[k+250][j];
                            s252 += a[i][k+251] * b[k+251][j];
                            s253 += a[i][k+252] * b[k+252][j];
                            s254 += a[i][k+253] * b[k+253][j];
                            s255 += a[i][k+254] * b[k+254][j];
                            s256 += a[i][k+255] * b[k+255][j];
                        }
                        c[i][j] += s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9 + s10 + s11 + s12 + s13 + s14 + s15 + s16 +
                                   s17 + s18 + s19 + s20 + s21 + s22 + s23 + s24 + s25 + s26 + s27 + s28 + s29 + s30 + s31 + s32 +
                                   s33 + s34 + s35 + s36 + s37 + s38 + s39 + s40 + s41 + s42 + s43 + s44 + s45 + s46 + s47 + s48 +
                                   s49 + s50 + s51 + s52 + s53 + s54 + s55 + s56 + s57 + s58 + s59 + s60 + s61 + s62 + s63 + s64 +
                                   s65 + s66 + s67 + s68 + s69 + s70 + s71 + s72 + s73 + s74 + s75 + s76 + s77 + s78 + s79 + s80 +
                                   s81 + s82 + s83 + s84 + s85 + s86 + s87 + s88 + s89 + s90 + s91 + s92 + s93 + s94 + s95 + s96 +
                                   s97 + s98 + s99 + s100 + s101 + s102 + s103 + s104 + s105 + s106 + s107 + s108 + s109 + s110 + s111 + s112 +
                                   s113 + s114 + s115 + s116 + s117 + s118 + s119 + s120 + s121 + s122 + s123 + s124 + s125 + s126 + s127 + s128 +
                                   s129 + s130 + s131 + s132 + s133 + s134 + s135 + s136 + s137 + s138 + s139 + s140 + s141 + s142 + s143 + s144 +
                                   s145 + s146 + s147 + s148 + s149 + s150 + s151 + s152 + s153 + s154 + s155 + s156 + s157 + s158 + s159 + s160 +
                                   s161 + s162 + s163 + s164 + s165 + s166 + s167 + s168 + s169 + s170 + s171 + s172 + s173 + s174 + s175 + s176 +
                                   s177 + s178 + s179 + s180 + s181 + s182 + s183 + s184 + s185 + s186 + s187 + s188 + s189 + s190 + s191 + s192 +
                                   s193 + s194 + s195 + s196 + s197 + s198 + s199 + s200 + s201 + s202 + s203 + s204 + s205 + s206 + s207 + s208 +
                                   s209 + s210 + s211 + s212 + s213 + s214 + s215 + s216 + s217 + s218 + s219 + s220 + s221 + s222 + s223 + s224 +
                                   s225 + s226 + s227 + s228 + s229 + s230 + s231 + s232 + s233 + s234 + s235 + s236 + s237 + s238 + s239 + s240 +
                                   s241 + s242 + s243 + s244 + s245 + s246 + s247 + s248 + s249 + s250 + s251 + s252 + s253 + s254 + s255 + s256;
                    }
                }
            }
        }
    }
    int EndMs = GetTickCount();
    std::cout << "Time for blockMultiply: " << (static_cast<double>(EndMs - StartMs) / 1000) << 
    " seconds;   BlockSize: " << b_size << ";   M = 256;   Size: " << size << "x" << size << "\n\n";
    return c;
}
////////////////////////////////////////////////////////////


int main()
{
    SetConsoleOutputCP(65001);
    std::cout << "Введите размер квадратной матрицы кратный степени 2:" << std::endl;
    int size = 0;
    std::cin >> size;
    std::cout << std::endl;

    std::vector<std::vector<double>> a = generator(size, -9.0, 9.0);
    std::vector<std::vector<double>> b = generator(size, -9.0, 9.0);

    std::cout << "Введите номер эксперимента:" << std::endl;
    std::cout << "  1) Классическое умножение" << std::endl;
    std::cout << "  2) Классическое и транспонированное с учетом и без учета времени транспонирования" << std::endl;
    std::cout << "  3) Буфферизированное умножение с распараллеливанием от 1 до 256" << std::endl;
    std::cout << "  4) Блочное умножение" << std::endl;
    std::cout << "  5) Блочное умножение с раскруткой внутренних циклов" << std::endl;
    std::cout << "  6) Лучшие реализации" << std::endl;
    int num_of_method = 0;
    std::cin >> num_of_method;

    switch(num_of_method) {
        case 1:
        {
            std::vector<std::vector<double>> c1(size, std::vector<double>(size));
            classicMultiply(a, b);
            break;
        }
        case 2:
        {
            std::vector<std::vector<double>> c1(size, std::vector<double>(size));
            std::vector<std::vector<double>> c2(size, std::vector<double>(size));
            std::vector<std::vector<double>> c3(size, std::vector<double>(size));
            //c1 = classicMultiply(a, b);
            c2 = withT_transposeMultiply(a, b);
            c3 = withoutT_transposeMultiply(a, b);

            //std::cout << (equal(c1,c2) ? "c1 и с2 равны" : "c1 и с2 не равны" )<< std::endl;
            //std::cout << (equal(c1,c3) ? "c1 и с3 равны" : "c1 и с3 не равны" )<< std::endl;
            break;
        }
        case 3:
        {
            std::vector<std::vector<double>> c1(size, std::vector<double>(size));
            std::vector<std::vector<double>> c2(size, std::vector<double>(size));
            std::vector<std::vector<double>> c3(size, std::vector<double>(size));
            std::vector<std::vector<double>> c4(size, std::vector<double>(size));
            std::vector<std::vector<double>> c5(size, std::vector<double>(size));
            std::vector<std::vector<double>> c6(size, std::vector<double>(size));
            std::vector<std::vector<double>> c7(size, std::vector<double>(size));
            std::vector<std::vector<double>> c8(size, std::vector<double>(size));
            std::vector<std::vector<double>> c9(size, std::vector<double>(size));
            c1 = buffMultiply1(a, b);
            c2 = buffMultiply2(a, b);
            c3 = buffMultiply4(a, b);
            c4 = buffMultiply8(a, b);
            c5 = buffMultiply16(a, b);
            c6 = buffMultiply32(a, b);
            c7 = buffMultiply64(a, b);
            c8 = buffMultiply128(a, b);
            c9 = buffMultiply256(a, b);

            std::cout << (equal(c1,c2) ? "c1 и с2 равны" : "c1 и с2 не равны" )<< std::endl;
            std::cout << (equal(c1,c3) ? "c1 и с3 равны" : "c1 и с3 не равны" )<< std::endl;
            std::cout << (equal(c1,c7) ? "c1 и с4 равны" : "c1 и с4 не равны" )<< std::endl;
            std::cout << (equal(c1,c8) ? "c1 и с5 равны" : "c1 и с5 не равны" )<< std::endl;
            std::cout << (equal(c1,c9) ? "c1 и с6 равны" : "c1 и с6 не равны" )<< std::endl;
            break;
        }
        case 4:
        {
            //std::vector<std::vector<double>> c1(size, std::vector<double>(size));
            //std::vector<std::vector<double>> c2(size, std::vector<double>(size));
            blockMultiply1(a, b, 1);
            blockMultiply1(a, b, 2);
            blockMultiply1(a, b, 4);
            blockMultiply1(a, b, 8);
            blockMultiply1(a, b, 16);
            blockMultiply1(a, b, 32);
            blockMultiply1(a, b, 64);
            blockMultiply1(a, b, 128);
            blockMultiply1(a, b, 256);
            //std::cout << (equal(c1,c2) ? "c1 и с2 равны" : "c1 и с2 не равны" ) << std::endl;
            break;
        }
        case 5:
        {
            std::cout << "Размер блока: ";
            int s = 0;
            std::cin >> s;
            switch(s) {
                case 1:
                    blockMultiply1(a, b, s);
                    break;
                case 2:
                    //blockMultiply1(a, b, s); 
                    blockMultiply2(a, b, s);
                    break;
                case 4:
                    //blockMultiply1(a, b, s); 
                    blockMultiply2(a, b, s);
                    blockMultiply4(a, b, s);
                    break;
                case 8:
                    //blockMultiply1(a, b, s); 
                    blockMultiply2(a, b, s);
                    blockMultiply4(a, b, s);
                    blockMultiply8(a, b, s);
                    break;
                case 16:
                    //blockMultiply1(a, b, s); 
                    blockMultiply2(a, b, s);
                    blockMultiply4(a, b, s);
                    blockMultiply8(a, b, s);
                    blockMultiply16(a, b, s);
                    break;
                case 32:
                    //blockMultiply1(a, b, s); 
                    blockMultiply2(a, b, s);
                    blockMultiply4(a, b, s);
                    blockMultiply8(a, b, s);
                    blockMultiply16(a, b, s);
                    blockMultiply32(a, b, s);
                    break;
                case 64:
                    //blockMultiply1(a, b, s); 
                    blockMultiply2(a, b, s);
                    blockMultiply4(a, b, s);
                    blockMultiply8(a, b, s);
                    blockMultiply16(a, b, s);
                    blockMultiply32(a, b, s);
                    blockMultiply64(a, b, s);
                    break;
                case 128:
                    //blockMultiply1(a, b, s); 
                    blockMultiply2(a, b, s);
                    blockMultiply4(a, b, s);
                    blockMultiply8(a, b, s);
                    blockMultiply16(a, b, s);
                    blockMultiply32(a, b, s);
                    blockMultiply64(a, b, s);
                    blockMultiply128(a, b, s);
                    break;
                case 256:
                    //blockMultiply1(a, b, s); 
                    blockMultiply2(a, b, s);
                    blockMultiply4(a, b, s);
                    blockMultiply8(a, b, s);
                    blockMultiply16(a, b, s);
                    blockMultiply32(a, b, s);
                    blockMultiply64(a, b, s);
                    blockMultiply128(a, b, s);
                    blockMultiply256(a, b, s);
                    break;
                default:
                    return 0;
            }
            break;
        }
        case 6:
            classicMultiply(a, b);
            withoutT_transposeMultiply(a, b);
            buffMultiply32(a, b);
            blockMultiply8(a, b, 256);
            break;

        default:
            std::cout << "Неправильный ввод" << std::endl;
            return 0;
    }





    return 0;
}

