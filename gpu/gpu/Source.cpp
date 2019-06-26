//#include <CL/opencl.h>
#include <stdio.h> 
#include <ctime> 
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#include "Prototype_test.h"


int main() {
	
		//Вектор хранящий время выполнения теста
	std::vector<double> time_vector(0);
	
		//Переменные для регистрации времени выполнения всех тестовтеста
	clock_t start_m, end_m;
		//Переменные для регистрации времени выполнения теста
	clock_t start, end;
	
	double seconds;
	
		//Количество тестов
	int pass = 1000;
	
		//Количество проходов в тесте
	int turn = 1000;

		//Входные вектора
	std::vector<int> a(0), b(0);
		//Инициализация входных векторов
	a = init_vector(1000, 200);
	b = init_vector(1000, 200);

		//Резервная полуавтоматическая инициализация входных векторов
	//int *size = new int(1000);
	//for (int i = 0; i < *size; i++)
	//{
	//	a.push_back(i + rand() % 100);
	//	b.push_back(i + rand() % 100);
	//}
	//delete size;

		//Главный цикл выполняюций заданное количество тестов
	start_m = clock();
	for (int i = 0; i < pass; i++)
	{
		start = clock();
		//test_load1(turn, a, b);
		test_load2(turn, a, b);
		//test_load3(turn, a, b);
		end = clock();

		seconds = time_calculate(start, end);
		time_vector.push_back(seconds);
		std::cout << i << "\n";
	}
	end_m = clock();

	print_statistic(end_m, start_m, turn, pass, time_vector);
	write_to_file(end_m, start_m, turn, pass, time_vector, "CPU", "1PC", "1kx1k_vector_mul.csv");

	system("pause");
}


