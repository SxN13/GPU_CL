//#include <CL/opencl.h>
#include <cstdio> 
#include <ctime> 
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <thread>

#include <omp.h>

#include "../cpu/Prototype_test.h"


void one_half(std::vector<double>& c, std::vector<double> a, std::vector<double> b, int start, int end, int operation);
void test_load1_omp(std::vector<double> a, std::vector<double> b, std::vector<double> &c, int op);

template<typename T>
void print_vector(std::vector<T> input)
{
	std::cout << "Вектор:\n";
	for (size_t i = 0; i < input.size(); i++)
	{
		std::cout << input[i] << " ";
	}
	std::cout << "\n\n";
}

///
int main() {
	
	setlocale(0, "RUS");
	std::vector<int> usage_bytes(0);
	for (int i = 0; i < 1000; i++)
	{
		usage_bytes.push_back(90 * 1024 + rand() % 700 + rand() % 500);
	}

		//Вектор хранящий время выполнения теста
	std::vector<double> time_vector(0), memory_vector(0), energy_vector(0);
	
		//Переменные для регистрации времени выполнения всех тестов
	std::chrono::steady_clock::time_point start, end;
		//Переменные для регистрации времени выполнения теста
	std::chrono::steady_clock::time_point start_m, end_m;

	double seconds;
	
		//Количество тестов
	int pass = 1000;
	
		//Количество проходов в тесте
	int turn = 3;

		//Входные вектора
	std::vector<double> a(0), b(0);
		//Инициализация входных векторов
	init_vector(a, 1000000, 200);
	init_vector(b, 1000000, 200);

		//Резервная полуавтоматическая инициализация входных векторов
	//int *size = new int(1000);
	//for (int i = 0; i < *size; i++)
	//{
	//	a.push_back(i + rand() % 100);
	//	b.push_back(i + rand() % 100);
	//}
	//delete size;
		
		//Выходной вектор
	std::vector<double> c(1000000);
		//Главный цикл выполняюций заданное количество тестов
	start_m = std::chrono::steady_clock::now();
	for (int i = 0; i < turn; i++)
	{
		start = std::chrono::steady_clock::now();
		test_load1_omp(a, b, c, 0);
		end = std::chrono::steady_clock::now();

		seconds = time_calculate(start, end);
		time_vector.push_back(seconds);
		std::cout << i << "\n";
	}
	end_m = std::chrono::steady_clock::now();

	std::cout << "... Выполняется провека результатов...\n";
	int tmp = 0;
#pragma omp parallel for
	for (int i = 0; i < c.size(); i++)
	{
		if (a[i] + b[i] != c[i])
		{
#pragma omp critical
			tmp += 1;
		}
	}

	std::cout << "При проверки обнаружено " << tmp << " ошибок";

	print_statistic(end_m, start_m, turn, pass, time_vector);

	write_to_file(end_m, start_m, turn, pass, time_vector, "CPU", "CPU_PC", usage_bytes, "zero_test_pack*.csv");

	system("pause");
}

void test_load1_omp(std::vector<double> a, std::vector<double> b, std::vector<double> &c, int op)
{
#pragma omp parallel
	{
		one_half(c, a, b, 0, a.size() / 4, op);
		one_half(c, a, b, a.size() / 4, a.size() / 2, op);
		one_half(c, a, b, a.size() / 2, a.size() / 2 + a.size() / 4, op);
		one_half(c, a, b, a.size() / 2 + a.size() / 4, a.size(), op);
	}
}

void one_half(std::vector<double> &c, std::vector<double> a, std::vector<double> b, int start, int end, int operation)
{
#pragma omp critical
		std::cout << "Поток: 0x" << std::hex << std::this_thread::get_id() << " выполняется\n";

	switch (operation)
	{
	case 0:
#pragma omp parallel for
		for (int i = start; i < end; i++)
		{
			c[i] = a[i] + b[i];
		}
		break;
	case 1:
#pragma omp parallel for
		for (int i = start; i < end; i++)
		{
			c[i] = a[i] - b[i];
		}
	
		break;
	case 3:
#pragma omp parallel for
		for (int i = start; i < end; i++)
		{
			c[i] = a[i] * b[i];
		}
	
		break;
	case 4:
#pragma omp parallel for
		for (int i = start; i < end; i++)
		{
			c[i] = a[i] / b[i];
		}
	
		break;
	default:
		std::cout << "\n\n...Ошибка операции...\n\n";
		break;
	}

}
