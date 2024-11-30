#include "Lab5.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <omp.h>

namespace lab5
{
	void sum_vectors_parallel(std::vector<int>& a, std::vector<int>& b, std::vector<int>& res)
	{
		int n = a.size();

#pragma omp parallel sections
		{
#pragma omp section
			{
#pragma omp parallel for
				for (int i = 0; i < n / 2; ++i)
				{
					res[i] = a[i] + b[i];
				}
			}
#pragma omp section
			{
#pragma omp parallel for
				for (int i = n / 2; i < n; ++i)
				{
					res[i] = a[i] + b[i];
				}
			}
		}
	}

	void task1()
	{
		int n = 1000000;
		std::vector<int> a(n, 1), b(n, 2), res(n, 0);

		std::cout << "–азмер массива: " << n << std::endl;

		omp_set_nested(0);
		auto start_time = omp_get_wtime();
		sum_vectors_parallel(a, b, res);
		auto end_time = omp_get_wtime();
		std::cout << "ѕараллельное выполнение без вложенного параллелизма: " << (end_time - start_time) << " секунд\n";

		omp_set_nested(1);
		start_time = omp_get_wtime();
		sum_vectors_parallel(a, b, res);
		end_time = omp_get_wtime();
		std::cout << "ѕараллельное выполнение c вложенным параллелизмом: " << (end_time - start_time) << " секунд\n";
	}

	void merge(std::vector<int>& arr, int start, int mid, int end)
	{
		std::vector<int> temp(end - start + 1);
		int i = start, j = mid + 1, k = 0;

		while (i <= mid && j <= end)
		{
			if (arr[i] <= arr[j]) {
				temp[k++] = arr[i++];
			}
			else {
				temp[k++] = arr[j++];
			}
		}
		while (i <= mid)
			temp[k++] = arr[i++];
		while (j <= end)
			temp[k++] = arr[j++];

		for (int i = 0; i < temp.size(); ++i)
		{
			arr[start + i] = temp[i];
		}
	}

	void sequential_merge_sort(std::vector<int>& arr, int start, int end)
	{
		if (start >= end)
			return;
		if (end - start == 1)
		{
			if (arr[start] > arr[end])
				std::swap(arr[start], arr[end]);
			return;
		}

		int mid = start + (end - start) / 2;
		sequential_merge_sort(arr, start, mid);
		sequential_merge_sort(arr, mid + 1, end);
		merge(arr, start, mid, end);
	}

	void parallel_merge_sort(std::vector<int>& arr, int start, int end)
	{
		if (start >= end)
			return;
		if (end - start == 1)
		{
			if (arr[start] > arr[end])
				std::swap(arr[start], arr[end]);
			return;
		}

		int mid = start + (end - start) / 2;
#pragma omp parallel sections
		{
#pragma omp section
			{
				parallel_merge_sort(arr, start, mid);
			}
#pragma omp section
			{
				parallel_merge_sort(arr, mid + 1, end);
			}
		}
		merge(arr, start, mid, end);
	}

	void task2()
	{
		srand(time(NULL));

		int n = 1000;
		std::vector<int> arr(n);
		for (int& i : arr)
			i = rand() % 100;

		std::vector<int> seq_arr = arr;
		std::vector<int> par_not_nested_arr = arr;
		std::vector<int> par_nested_arr = arr;

		std::cout << "–азмер массива: " << n << std::endl;

		double start_time = omp_get_wtime();
		sequential_merge_sort(seq_arr, 0, n - 1);
		double end_time = omp_get_wtime();
		std::cout << "ѕоследовательна€ сортировка сли€нием: " << (end_time - start_time) << " секунд\n";


		omp_set_nested(0);
		start_time = omp_get_wtime();
		parallel_merge_sort(par_not_nested_arr, 0, n - 1);
		end_time = omp_get_wtime();
		std::cout << "ѕараллельна€ сортировка сли€нием без вложенного параллелизма: " << (end_time - start_time) << " секунд\n";

		omp_set_nested(1);
		start_time = omp_get_wtime();
		parallel_merge_sort(par_nested_arr, 0, n - 1);
		end_time = omp_get_wtime();
		std::cout << "ѕараллельна€ сортировка сли€нием с вложенным параллелизмом: " << (end_time - start_time) << " секунд\n";
	}

	int partition(std::vector<int>& arr, int start, int end)
	{
		int pivot = arr[end];
		int i = start - 1;

		for (int j = start; j < end; ++j)
		{
			if (arr[j] < pivot)
				std::swap(arr[++i], arr[j]);
		}

		std::swap(arr[i + 1], arr[end]);

		return i + 1;
	}

	void sequential_quick_sort(std::vector<int>& arr, int start, int end)
	{
		if (end <= start)
			return;
		if (end - start == 1)
		{
			if (arr[start] > arr[end])
				std::swap(arr[start], arr[end]);
			return;
		}

		int pi = partition(arr, start, end);
		sequential_quick_sort(arr, start, pi - 1);
		sequential_quick_sort(arr, pi + 1, end);

	}

	void parallel_quick_sort(std::vector<int>& arr, int start, int end) {
		if (start >= end) return;

		if (end - start == 1) {
			if (arr[start] > arr[end])
				std::swap(arr[start], arr[end]);
			return;
		}

		int pi = partition(arr, start, end);

#pragma omp parallel sections
		{
#pragma omp section
			{
				parallel_quick_sort(arr, start, pi - 1);
			}

#pragma omp section
			{
				parallel_quick_sort(arr, pi + 1, end);
			}
		}
	}

	void task3()
	{
		const int n = 1000;
		std::vector<int> arr(n);
		for (int& i : arr)
			i = rand() % 100;

		std::vector<int> seq_arr = arr;
		std::vector<int> par_not_nested_arr = arr;
		std::vector<int> par_nested_arr = arr;

		std::cout << "–азмер массива: " << n << std::endl;

		double start_time = omp_get_wtime();
		sequential_quick_sort(seq_arr, 0, n - 1);
		double end_time = omp_get_wtime();
		std::cout << "ѕоследовательна€ быстра€ сортировка: " << (end_time - start_time) << " секунд\n";

		omp_set_nested(0);
		start_time = omp_get_wtime();
		parallel_quick_sort(par_not_nested_arr, 0, n - 1);
		end_time = omp_get_wtime();
		std::cout << "ѕараллельна€ быстра€ сортировка без вложенного параллелизма: " << (end_time - start_time) << " секунд\n";

		omp_set_nested(1);
		start_time = omp_get_wtime();
		parallel_quick_sort(par_nested_arr, 0, n - 1);
		end_time = omp_get_wtime();
		std::cout << "ѕараллельна€ быстра€ сортировка с вложенным параллелизмом: " << (end_time - start_time) << " секунд\n";
	}
}