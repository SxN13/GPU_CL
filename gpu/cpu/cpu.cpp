//#include <CL/opencl.h>
#include <cstdio> 
#include <ctime> 
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#include "Prototype_test.h"


int main() {
	
	setlocale(0, "RUS");
	std::cout << "...Инициализация необходимых переменных...\n";

	std::vector<int> usage_bytes(0);
	for (int i = 0; i < 1000; i++)
	{
		usage_bytes.push_back(34 * 1024 + rand() % 700 + rand() % 500);
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
	int turn = 1000000;

		//Входные вектора
	std::vector<int> a(0), b(0);
		//Инициализация входных векторов
	if (init_vector(a, 1000000, 200) == 0) std::cout << "...Вектор а инициализирован и заполнен.\n";
	if (init_vector(b, 1000000, 200) == 0) std::cout << "...Вектор b инициализирован и заполнен.\n\n";
	//Выходной вектор
	std::vector<int> c(1000000);
		//Резервная полуавтоматическая инициализация входных векторов
	//int *size = new int(1000);
	//for (int i = 0; i < *size; i++)
	//{
	//	a.push_back(i + rand() % 100);
	//	b.push_back(i + rand() % 100);
	//}
	//delete size;

		//Главный цикл выполняюций заданное количество тестов
	start_m = std::chrono::steady_clock::now();
	for (int i = 0; i < pass; i++)
	{
		start = std::chrono::steady_clock::now();
		//test_load1(turn, a, b);
		//test_load2(turn, a, b);
		test_load3(c, a, b);
		end = std::chrono::steady_clock::now();

		seconds = time_calculate(start, end);
		time_vector.push_back(seconds);
		std::cout << i << " ";
	}
	end_m = std::chrono::steady_clock::now();

	//Проверка результатов
	int tmp = 0;
	std::string str = "";
	for (size_t i = 0; i < c.size(); i++)
	{
		if (b[i] == 0) b[i] = 1;
		if (a[i] / b[i] != c[i])
		{
			tmp += 1;
			str += "\n";
			str += std::to_string(a[i]);
			str += " + ";
			str += std::to_string(b[i]);
			str += " = ";
			str += std::to_string(c[i]);
			str += "\n";
		}
	}

	std::cout << "\n...Проверка результатов...\n" << "...При проверки обнаружено " << tmp << " ошибок." << str;


	print_statistic(end_m, start_m, turn, pass, time_vector);

	std::cout << "\n\n...Запись в файла...\t";
	write_to_file(0,3,1,end_m,start_m, time_vector, usage_bytes, "13.csv");
	std::cout << " Завершена\n";

	system("pause");
}


