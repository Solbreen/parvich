#include <vector>
#include <random>
#include <iostream>
#include <cmath>



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
        //std::cout << "Не равны" << std::endl;
        return false;
    }

    for (size_t i = 0; i < a.size(); i++)
    {
        if (a[i].size() != b[i].size())
        {
            //std::cout << "Не равны" << std::endl;
            return false;
        }

        for (size_t j = 0; j < a[i].size(); j++)
        {
            if (std::fabs(a[i][j] - b[i][j]) > eps)
                //std::cout << "Не равны" << std::endl;
                return false;
        }
    }
    //std::cout << "Равны" << std::endl;
    return true;
}

std::vector<std::vector<double>> classicMultiply(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b)
{   
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    // начало замера
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
    //конец замера
    //вывод времени
    return c;
}

std::vector<std::vector<double>> transposeMultiply(const std::vector<std::vector<double>>& a, std::vector<std::vector<double>> b)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    // начало замера
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
    // конец замера
    // вывод времени
    return c;
}

std::vector<std::vector<double>> buffMultiply(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));
    // начало замера
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
    // конец замера
    // вывод времени
    return c;
}

std::vector<std::vector<double>> blockMultiply(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b)
{
    int size = a.size();
    std::vector<std::vector<double>> c(size, std::vector<double>(size, 0.0));

    return c;
}

int main()
{
    int size = 512;
    std::vector<std::vector<double>> a = generator(size, -9.0, 9.0);
    std::vector<std::vector<double>> b = generator(size, -9.0, 9.0);
    std::vector<std::vector<double>> c1(size, std::vector<double>(size));
    std::vector<std::vector<double>> c2(size, std::vector<double>(size));
    std::vector<std::vector<double>> c3(size, std::vector<double>(size));
    std::vector<std::vector<double>> c4(size, std::vector<double>(size));

    c1 = classicMultiply(a, b);
    c2 = transposeMultiply(a, b);
    c3 = buffMultiply(a, b);
    c4 = blockMultiply(a, b);
    //mat(c1, size);
    //mat(c2, size);
    //mat(c3, size);

    std::cout << (equal(c1,c2) ? "Равны" : "Не равны" )<< std::endl;
    std::cout << (equal(c1,c3) ? "Равны" : "Не равны" )<< std::endl;
    return 0;
}

