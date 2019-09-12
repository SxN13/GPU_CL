//#include <CL/opencl.h>
#include <stdio.h> 
#include <ctime> 
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#include <thread>
#include <mutex>

#include "../cpu/Prototype_test.h"

///

void one_half(std::vector<int>& c, std::vector<int> a, std::vector<int> b, int start, int end, int operation);

void test_load1_thread(std::vector<int> a, std::vector<int> b, std::vector<int> &c);

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

std::mutex mutx;

int main() {
	
	setlocale(0, "RUS");
	std::cout << "\n...Инициализация необходимых переменных...\n";

	std::vector<int> usage_bytes(0);
	for (int i = 0; i < 1000; i++)
	{
		usage_bytes.push_back(50 * 1024 + rand() % 700 + rand() % 500);
	}

		//Вектор хранящие параметры выполнения теста
	std::vector<int> time_vector(0), memory_vector(0), energy_vector(0);
	
		//Переменные для регистрации времени выполнения всех тестов
	std::chrono::steady_clock::time_point start, end;
		//Переменные для регистрации времени выполнения теста
	std::chrono::steady_clock::time_point start_m, end_m;

	double seconds;
	
		//Количество тестов
	int pass = 1000;

		//Входные вектора
	std::vector<int> a(0), b(0);
		//Инициализация входных векторов
	if (init_vector(a, 1000000, 200) == 0) std::cout << "...Вектор а инициализирован и заполнен.";
	if (init_vector(b, 1000000, 200) == 0) std::cout << "...Вектор b инициализирован и заполнен.\n\n";

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

		test_load1_thread(a, b, c);

		end = std::chrono::steady_clock::now();

		seconds = time_calculate(start, end);
		time_vector.push_back(seconds);
		std::cout << std::dec << i << "\n";
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

	print_statistic(end_m, start_m, c.size(), pass, time_vector);

	std::cout << "\n\n...Запись в файла...\t";
	write_to_file(1, 3, 4, end_m, start_m, time_vector, usage_bytes, "113.csv");
	std::cout << " Завершена\n";

	system("pause");
}

void test_load1_thread(std::vector<int> a, std::vector<int> b, std::vector<int> &c)
{
	
	std::thread thread1(one_half, std::ref(c), a, b, 0, a.size() / 4, 3);
	std::thread thread2(one_half, std::ref(c), a, b, (a.size() / 4), (a.size() / 2), 3);
	std::thread thread3(one_half, std::ref(c), a, b, (a.size() / 2), (a.size() / 2 + a.size() / 4), 3);
	std::thread thread4(one_half, std::ref(c), a, b, (a.size() / 2 + a.size() / 4), a.size(), 3);
	
	/*
	std::thread thread1(one_half, std::ref(c), a, b, 0, a.size() / 2, 0);
	std::thread thread2(one_half, std::ref(c), a, b, (a.size() / 2), a.size(), 0);
	*/
	thread1.join();
	thread2.join();

	
	thread3.join();
	thread4.join();
	
}

void one_half(std::vector<int> &c, std::vector<int> a, std::vector<int> b, int start, int end, int operation)
{
	
	mutx.lock();
	std::cout << "Поток: 0x" << std::hex << std::this_thread::get_id() << " выполняется\n";
	mutx.unlock();
	switch (operation)
	{
	case 0:
		for (size_t i = start; i < end; i++)
		{

			c[i] = a[i] + b[i];
		}
		break;
	case 1:
		for (size_t i = start; i < end; i++)
		{
			c[i] = a[i] - b[i];
		}
		break;
	case 2:
		for (size_t i = start; i < end; i++)
		{
			c[i] = a[i] * b[i];
		}
		break;
	case 3:
		for (size_t i = start; i < end; i++)
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
