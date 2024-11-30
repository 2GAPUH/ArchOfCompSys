#include <stdlib.h>
#include <iostream>
#include <omp.h>
#include "Lab4.h"

namespace lab0
{
    void task1()
    {
        short a = 99, b = a, c, C;
        c = a + b;

        _asm {
            mov ax, a
            add ax, b
            mov C, ax

            jo overflow_flag
        }

        std::cout << c << "     " << C << std::endl;
        goto cont;

    overflow_flag:
        std::cout << "overflow" << std::endl;

    cont:
        return;
    }

    void task2()
    {
        double a = 24.1, b = -17.5, c = -21.2, _2 = 2, _4 = 4;
        double D, sqrtD, x1, x2;

        __asm
        {
            fld b
            fmul b
            fld a
            fld c
            fmul
            fmul _4
            fsub
            fstp D
        }

        if (D < 0)
            std::cout << "Корней нет" << std::endl;

        __asm
        {
            fld D
            fsqrt
            fstp sqrtD
        }

        __asm
        {
            fld b
            fchs
            fld sqrtD
            fadd
            fld a
            fmul _2
            fdiv
            fstp x1

            fld b
            fchs
            fld sqrtD
            fsub
            fld a
            fmul _2
            fdiv
            fstp x2
        }

        std::cout << x1 << "       " << x2 << std::endl;
    }

    void task3()
    {
        double a = 1.0, b = -6.0, c = 11.0, d = -6.0;
        double tolerance = 1e-6;
        int max_iterations = 1000;
        double x0 = 1.0;
        double _3 = 3, _2 = 2;
        double x1, x2, x3;
        double fx, dfx;

        for (int i = 0; i < max_iterations; ++i)
        {
            __asm
            {
                fld x0
                fld x0
                fmul
                fld x0
                fmul
                fld a
                fmul
                fld x0
                fld x0
                fmul
                fld b
                fmul
                fadd
                fld x0
                fld c
                fmul
                fadd
                fld d
                fadd
                fstp fx

                fld x0
                fld x0
                fmul
                fld a
                fmul
                fld _3
                fmul
                fld x0
                fld b
                fmul
                fld _2
                fmul
                fadd
                fld c
                fadd
                fstp dfx
            }

            if (fabs(dfx) < 1e-12)
            {
                std::cout << "Невозможно вычислить" << std::endl;
                return;
            }

            __asm
            {
                fld fx
                fld dfx
                fdiv
                fld x0
                fsub
                fstp x1
            }

            if (fabs(x1 - x0) < tolerance)
                break;

            x0 = x1;
        }

        double A = a;
        double B = b + a * x1;
        double C = c + b * x1 + a * x1 * x1;
        double sqrtD;
        double _4 = 4.0;

        __asm
        {
            fld B
            fmul B
            fld A
            fld C
            fmul
            fmul _4
            fsub
            fsqrt
            fstp sqrtD
        }

        __asm
        {
            fld B
            fchs
            fld sqrtD
            fadd
            fld A
            fmul _2
            fdiv
            fstp x2

            fld B
            fchs
            fld sqrtD
            fsub
            fld A
            fmul _2
            fdiv
            fstp x3
        }

        std::cout << x1 << "    " << x2 << "     " << x3 << std::endl;
    }
}

namespace lab1
{
    void task1()
    {
        std::cout << "Последовательная область 1\n";

        #pragma omp parallel
        {
            std::cout << "Параллельная область\n";
        }

        std::cout << "Последовательная область 2\n";
    }

    void task2()
    {
        #pragma omp parallel
        {
            int thread_id = omp_get_thread_num();

            if (thread_id == 0) {
                std::cout << "Кол-во порожденных нитей: " << omp_get_num_threads() << std::endl;
            }
            else {
                std::cout << "Нить: " << omp_get_thread_num() << std::endl;
            }
        }
    }

    void task3()
    {
        #pragma omp parallel
        {
            std::cout << "Параллельный регион 1\n";
        }

        #pragma omp parallel num_threads(3)
        {
            std::cout << "Параллельный регион 2\n";
        }

        omp_set_num_threads(4);
        #pragma omp parallel
        {
            std::cout << "Параллельный регион 3\n";
        }
    }

    void task4()
    {
        // Нединамический режим
        omp_set_dynamic(0);
        std::cout << "Нединамический режим\n";
        omp_set_num_threads(128);

        #pragma omp parallel 
        {
            #pragma omp master
            {
                std::cout << "Количество нитей: " << omp_get_num_threads() << std::endl;
            }
        }

        omp_set_dynamic(1);
        std::cout << "Динамический режим\n";

        #pragma omp parallel num_threads(128)
        {
            #pragma omp master
            {
                std::cout << "Количество нитей: " << omp_get_num_threads() << std::endl;
            }
        }
    }

    void task5()
    {
        omp_set_num_threads(4);

        std::string choice;
        std::cout << "Введите параметр \"parallel\" или \"serial\"\n";
        std::cin >> choice;

        if (choice == "serial")
        {
            std::cout << "Последовательный регион\n";
        }
        else
        {
            #pragma omp parallel
            {
                std::cout << "Параллельный регион\n";
            }
        }
    }

}

namespace lab2
{
    void task1()
    {
        const int iterations = 1000000;
        double num1, num2;
        std::cout << "Введите два вещественных числа: num1 num2\n";
        std::cin >> num1 >> num2;

        double result = 0;
        double start_time = omp_get_wtime();

        for (int i = 0; i < iterations; ++i) {
            result = num1 * num2;
        }
        double end_time = omp_get_wtime();
        std::cout << "Время на однопоточные вычисления: " << (end_time - start_time) << " секунд\n";

        result = 0;
        start_time = omp_get_wtime();

        #pragma omp parallel num_threads(2)
        for (int i = 0; i < iterations; ++i) {
            result = num1 * num2;
        }

        end_time = omp_get_wtime();
        std::cout << "Время на двухпоточные вычисления: " << (end_time - start_time) << " секунд\n";
    }

    void task2()
    {
        double start_time = omp_get_wtime();

        #pragma omp parallel
        {
            // Пустая параллельная область
        }

        double end_time = omp_get_wtime();
        std::cout << "Время выполнения служебных операций: " << (end_time - start_time) << " секунд\n";
    }

    void task3()
    {
        #pragma omp parallel num_threads(3)
        {
            int thread_num = omp_get_thread_num();
            #pragma omp critical
            {
                std::cout << "Начало, нить: " << thread_num << std::endl;
            }
            #pragma omp single nowait
            {
                #pragma omp critical
                {
                    std::cout << "Одна нить: " << thread_num << std::endl;
                }
            }
            #pragma omp critical
            {
                std::cout << "Окончание, нить: " << thread_num << std::endl;
            }
        }

        std::cout << "\n\n";

        #pragma omp parallel num_threads(3)
        {
            int thread_num = omp_get_thread_num();
            #pragma omp critical
            {
                std::cout << "Начало, нить: " << thread_num << std::endl;
            }

            #pragma omp single nowait
            {
                #pragma omp critical
                {
                    std::cout << "Одна нить: " << thread_num << std::endl;
                }
            }

            #pragma omp critical
            {
                std::cout << "Окончание, нить: " << thread_num << std::endl;
            }
        }
    }

    void task4()
    {
        #pragma omp parallel num_threads(3)
        {
            int thread_num = omp_get_thread_num();
            #pragma omp critical
            {
                std::cout << "Начало, нить: " << thread_num << std::endl;
            }
            #pragma omp master 
            {
                #pragma omp critical
                {
                    std::cout << "Главный поток, нить: " << thread_num << std::endl;
                }
            }
            #pragma omp critical
            {
                std::cout << "Середина, нить: " << thread_num << std::endl;
                std::cout << "Конец, нить: " << thread_num << std::endl;
            }
        }
    }

    void task5()
    {
        int n = 10;
        std::cout << "Последовательная область: n = " << n << std::endl;
        omp_set_num_threads(2);

        #pragma omp parallel private(n)
        {
            #pragma omp critical
            {
                int thread_num = omp_get_thread_num();
                std::cout << "Параллельная область (до присвоения), нить " << thread_num << ", n = " << n << std::endl;
                n = thread_num;
                std::cout << "Параллельная область (после присвоения), нить " << thread_num << ", n = " << n << std::endl;
            }
        }

        std::cout << "Последовательная область: n = " << n << std::endl;
    }

    void task6()
    {
        int m[5] = { 0 };
        std::cout << "Исходный массив: ";
        for (int i = 0; i < 5; ++i) 
            std::cout << m[i] << " ";
        std::cout << std::endl;

        #pragma omp parallel num_threads(2) shared(m)
        {
            int thread_num = omp_get_thread_num();
            m[thread_num] = 1;
        }

        std::cout << "Измененный массив: ";
        for (int i = 0; i < 5; ++i) 
            std::cout << m[i] << " ";
        std::cout << std::endl;
    }

    void task7()
    {
        int thread_count = 0;

        #pragma omp parallel num_threads(4) reduction(+:thread_count)
        {
            int thread_num = omp_get_thread_num();

            #pragma omp critical
            {
                thread_count += 1;
                std::cout << "Текущий поток: " << thread_num << " Текущее значение: " << thread_count << std::endl;
            }
        }

        std::cout << "Общее количество потоков: " << thread_count << std::endl;
    }

    void task8()
    {
        int thread_sum = 0;

        #pragma omp parallel reduction(+:thread_sum) num_threads(4)
        {
            int thread_num = omp_get_thread_num();

            #pragma omp critical
            {
                thread_sum += thread_num;
                std::cout << "Текущий поток: " << thread_num << " Текущая сумма: " << thread_sum << std::endl;
            }
        }

        std::cout << "Итоговая сумма номеров потоков: " << thread_sum << std::endl;
    }

}

int main()
{
    std::system("chcp 1251");

    lab4::start();

	return 0;
}