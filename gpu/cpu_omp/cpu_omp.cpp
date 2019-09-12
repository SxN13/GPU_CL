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


void one_half(std::vector<int> &c, std::vector<int> a, std::vector<int> b, int start, int end, int operation);
void test_load1_omp(std::vector<int> a, std::vector<int> b, std::vector<int> &c, int op);



///
int main() {
	
	setlocale(0, "RUS");
	std::vector<int> usage_bytes(0);
	for (int i = 0; i < 1000; i++)
	{
		usage_bytes.push_back(67 * 1024 + rand() % 700 + rand() % 500);
	}

		//Вектор хранящий время выполнения теста
	std::vector<int> time_vector(0), memory_vector(0), energy_vector(0);
	
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
	std::vector<int> a(0), b(0);
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
	std::vector<int> c(1000000);
		//Главный цикл выполняюций заданное количество тестов
	start_m = std::chrono::steady_clock::now();
	for (int i = 0; i < pass; i++)
	{
		start = std::chrono::steady_clock::now();
		test_load1_omp(a, b, c, 3);
		end = std::chrono::steady_clock::now();

		seconds = time_calculate(start, end);
		time_vector.push_back(seconds);
		std::cout << i << " ";
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

	std::cout << "\n\n...Запись в файла...\t";
	write_to_file(2, 3, 16, end_m, start_m, time_vector, usage_bytes, "123.csv");
	std::cout << " Завершена\n";

	system("pause");
	return 0;
}

void test_load1_omp(std::vector<int> a, std::vector<int> b, std::vector<int> &c, int op)
{
#pragma omp parallel
	{
		one_half(c, a, b, 0, a.size() / 4, op);
		one_half(c, a, b, a.size() / 4, a.size() / 2, op);
		one_half(c, a, b, a.size() / 2, a.size() / 2 + a.size() / 4, op);
		one_half(c, a, b, a.size() / 2 + a.size() / 4, a.size(), op);
	}
}

void one_half(std::vector<int> &c, std::vector<int> a, std::vector<int> b, int start, int end, int operation)
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
	case 2:
#pragma omp parallel for
		for (int i = start; i < end; i++)
		{
			c[i] = a[i] * b[i];
		}
	
		break;
	case 3:
#pragma omp parallel for
		for (int i = start; i < end; i++)
		{
			if (b[i] == 0) b[i] = 1;
			c[i] = a[i] / b[i];
		}
	
		break;
	default:
		std::cout << "\n\n...Ошибка операции...\n\n";
		break;
	}

}
