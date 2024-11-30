#include <iostream>
#include <cmath>
#include <omp.h>
#include "Lab4.h"

namespace lab4
{
    double computeFunction(double x)
    {
        return log(1.2 + tan(x / 2));
    }

    double integrateLeft(int divisions, double start, double end)
    {
        double total = 0.0;
        double step = (end - start) / divisions;
        for (int i = 0; i < divisions; ++i) {
            double current = start + i * step;
            total += computeFunction(current) * step;
        }
        return total;
    }

    double integrateLeftParallel(int divisions, double start, double end)
    {
        double total = 0.0;
        double step = (end - start) / divisions;

        #pragma omp parallel for reduction(+:total)
        for (int i = 0; i < divisions; ++i) {
            double current = start + i * step;
            total += computeFunction(current) * step;
        }

        return total;
    }

    double integrateRight(int divisions, double start, double end)
    {
        double total = 0.0;
        double step = (end - start) / divisions;
        for (int i = 1; i <= divisions; ++i) {
            double current = start + i * step;
            total += computeFunction(current) * step;
        }
        return total;
    }

    double integrateRightParallel(int divisions, double start, double end)
    {
        double total = 0.0;
        double step = (end - start) / divisions;

        #pragma omp parallel for reduction(+:total)
        for (int i = 1; i <= divisions; ++i) {
            double current = start + i * step;
            total += computeFunction(current) * step;
        }

        return total;
    }

    double integrateMid(int divisions, double start, double end)
    {
        double total = 0.0;
        double step = (end - start) / divisions;
        for (int i = 0; i < divisions; ++i) {
            double current = start + (i + 0.5) * step;
            total += computeFunction(current) * step;
        }
        return total;
    }

    double integrateMidParallel(int divisions, double start, double end)
    {
        double total = 0.0;
        double step = (end - start) / divisions;

        #pragma omp parallel for reduction(+:total)
        for (int i = 0; i < divisions; ++i) {
            double current = start + (i + 0.5) * step;
            total += computeFunction(current) * step;
        }

        return total;
    }

    double integrateTrapezoid(int divisions, double start, double end)
    {
        double total = 0.0;
        double step = (end - start) / divisions;
        total += (computeFunction(start) + computeFunction(end)) / 2.0;
        for (int i = 1; i < divisions; ++i) {
            double current = start + i * step;
            total += computeFunction(current);
        }
        return total * step;
    }

    double integrateTrapezoidParallel(int divisions, double start, double end)
    {
        double total = 0.0;
        double step = (end - start) / divisions;

        total += (computeFunction(start) + computeFunction(end)) / 2.0;

        #pragma omp parallel for reduction(+:total)
        for (int i = 1; i < divisions; ++i) {
            double current = start + i * step;
            total += computeFunction(current);
        }

        return total * step;
    }

    double integrateSimpson(int divisions, double start, double end)
    {
        if (divisions % 2 != 0) divisions++;
        double total = computeFunction(start) + computeFunction(end);
        double step = (end - start) / divisions;

        for (int i = 1; i < divisions; i += 2) {
            total += 4.0 * computeFunction(start + i * step);
        }
        for (int i = 2; i < divisions - 1; i += 2) {
            total += 2.0 * computeFunction(start + i * step);
        }
        return total * step / 3.0;
    }

    double integrateSimpsonParallel(int divisions, double start, double end)
    {
        if (divisions % 2 != 0) divisions++;
        double total = 0.0;
        double step = (end - start) / divisions;

        total += computeFunction(start) + computeFunction(end);

        #pragma omp parallel for reduction(+:total)
        for (int i = 1; i < divisions; i += 2) {
            total += 4.0 * computeFunction(start + i * step);
        }

        #pragma omp parallel for reduction(+:total)
        for (int i = 2; i < divisions; i += 2) {
            total += 2.0 * computeFunction(start + i * step);
        }

        return total * step / 3.0;
    }

    double calculateIntegral(int mode, int divisions, double start, double end)
    {
        switch (mode)
        {
        case 1: return integrateLeft(divisions, start, end);
        case 2: return integrateRight(divisions, start, end);
        case 3: return integrateMid(divisions, start, end);
        case 4: return integrateTrapezoid(divisions, start, end);
        case 5: return integrateSimpson(divisions, start, end);
        default: return 0.0;
        }
    }

    double calculateIntegralParallel(int mode, int divisions, double start, double end)
    {
        switch (mode)
        {
        case 1: return integrateLeftParallel(divisions, start, end);
        case 2: return integrateRightParallel(divisions, start, end);
        case 3: return integrateMidParallel(divisions, start, end);
        case 4: return integrateTrapezoidParallel(divisions, start, end);
        case 5: return integrateSimpsonParallel(divisions, start, end);
        default: return 0.0;
        }
    }

    void start()
    {
        system("cls");
        const double a = 0.0; 
        const double b = 2.0;
        const double expectedValue = 1.13977; 
        int n = 10;                           
        const int maxN = 100000000;           

        int method;
        std::cout << "=== Методы численного интегрирования ===\n";
        std::cout << "1. Метод левых прямоугольников\n";
        std::cout << "2. Метод правых прямоугольников\n";
        std::cout << "3. Метод средних прямоугольников\n";
        std::cout << "4. Метод трапеций\n";
        std::cout << "5. Метод Симпсона\n";
        std::cout << "========================================\n";
        std::cout << "Выберите метод (1-5): ";
        std::cin >> method;

        while (n <= maxN)
        {
            double tStart = omp_get_wtime();
            double sequentialResult = calculateIntegral(method, n, a, b);
            double sequentialTime = omp_get_wtime() - tStart;

            tStart = omp_get_wtime();
            double parallelResult = calculateIntegralParallel(method, n, a, b);
            double parallelTime = omp_get_wtime() - tStart;

            std::cout << "\n-------------------- РАЗБИЕНИЯ: " << n << " --------------------\n";
            std::cout << "Результаты:\n";
            std::cout << "   Последовательно: значение = " << std::fixed << std::setprecision(6) << sequentialResult
                << ", время = " << sequentialTime << " сек.\n";
            std::cout << "   Параллельно    : значение = " << parallelResult
                << ", время = " << parallelTime << " сек.\n";
            std::cout << "Погрешность: " << fabs(parallelResult - expectedValue) << "\n";
            std::cout << "Ускорение: " << std::setprecision(2) << (sequentialTime / parallelTime) << "x\n";
            std::cout << "-------------------------------------------------------------\n";

            n *= n;
        }
    }
}
