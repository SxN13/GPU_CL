#pragma once

#include <stdio.h> 
#include <ctime> 
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <windows.h>
#include <Psapi.h>
#pragma comment( lib, "psapi.lib" )

//////Прототипы функций//////

//Перегруженные операторы + для сложения векторов
template<typename T>
std::vector<T> operator+ (std::vector<T> a, std::vector<T> b);
template<typename T>
std::vector<T> operator- (std::vector<T> a, std::vector<T> b);
template<typename T>
std::vector<T> operator* (std::vector<T> a, std::vector<T> b);
//Возвращает среднее значение выходной выборки
template<typename T>
double average(std::vector<T> v);
//Запись в файл csv результатов проведенных тестов
void write_to_file(

	int test,
	int operation,
	int n_thread,
	std::chrono::steady_clock::time_point end_m,
	std::chrono::steady_clock::time_point start_m,
	std::vector<int> v,
	std::vector<int> bytes,
	std::string file_name

);

void test_load3(std::vector<int> &c, std::vector<int> a, std::vector<int> b);
//Возвращает заполненый вектор с размером size, значение элемента - порядковый номер + смещение seed
int init_vector(std::vector<double>& vector, int size, int seed);
//Возвращает максимальный элемент в выходной выборке
template<typename T>
double find_max(std::vector<T> v);
//Возвращает минимальный элемент в выходной выборке
template<typename T>
double find_min(std::vector<T> v);
//Вывод на экран статистики тестов
template<typename T>
void print_statistic(clock_t end_m, clock_t start_m, int turn, int pass, std::vector<T> v);
//Расчет времени выполнения
double time_calculate(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end);
//Расчет энергии
void calculate_energy(std::vector<double>& energy_vector, std::vector<double> time_vector, int proc);
//Получение данных по памяти устройства
void calculate_memory(DWORDLONG& totalVirtualMem, DWORDLONG& totalPhysMem, SIZE_T& physMemUsedByCurProc);
///////////////////////////////////////////////////////////////////


double time_calculate(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end)
{
	auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	return time.count();
}

template<typename T>
std::vector<T> operator+ (std::vector<T> a, std::vector<T> b)
{
	std::vector<T> c(0);
	for (int i = 0; i < a.size(); i++)
	{
		c.push_back(a[i] + b[i]);
	}
	return c;
}

template<typename T>
std::vector<T> operator- (std::vector<T> a, std::vector<T> b)
{
	std::vector<T> c(0);
	for (int i = 0; i < a.size(); i++)
	{
		c.push_back(a[i] - b[i]);
	}
	return c;
}

template<typename T>
std::vector<T> operator/ (std::vector<T> a, std::vector<T> b)
{
	std::vector<T> c(0);
	for (int i = 0; i < a.size(); i++)
	{
		if (b[i] == 0)
		{
			c.push_back(a[i] / 1);
		}
		else
		{
			c.push_back(a[i] / b[i]);
		}
	}
	return c;
}

template<typename T>
std::vector<T> operator* (std::vector<T> a, std::vector<T> b)
{
	std::vector<T> c(0);
	for (int i = 0; i < a.size(); i++)
	{
		c.push_back(a[i] * b[i]);
	}
	return c;
}

template<typename T>
double average(std::vector<T> v)
{
	double tmp = 0.;
	for (int i = 0; i < v.size(); i++)
	{
		tmp += v[i];
	}
	return tmp / v.size();
}

void write_to_file(

	int test,
	int operation,
	int n_thread,
	std::chrono::steady_clock::time_point end_m,
	std::chrono::steady_clock::time_point start_m,
	std::vector<int> v,
	std::vector<int> bytes,
	std::string file_name
	
	)
{
	std::ofstream file_out;
	file_out.open(file_name);

	file_out << "n_test," << "operation," << "n_thread," << "all_time," << "test_time," << "bytes," << "\n";
	std::string tmp = "";
	for (int i = 0; i < v.size(); i++)
	{
		file_out << test << ","
			<< operation << ","
			<< n_thread << ","
			<< time_calculate(end_m, start_m) << ","
			<< v[i] << ","
			<< bytes[i] << "\n";
	}
	file_out.close();
}

void test_load3(std::vector<int> &c, std::vector<int> a, std::vector<int> b)
{
	// turn -> количество проходов цикла сложения векторов
	//А и Б - входные векторы для умножения
	c = a / b;
}


int init_vector(std::vector<int>& vector, int size, int seed)
{
	srand(seed);
	for (size_t i = 0; i < size; i++)
	{
		vector.push_back(rand() % 255);
	}
	return 0;
}

template<typename T>
double find_max(std::vector<T> v)
{
	double out = 0.;
	for (int i = 0; i < v.size(); i++)
	{
		if (out <= v[i]) out = v[i];
	}

	return out;
}

template<typename T>
double find_min(std::vector<T> v)
{
	double out = 1000000.;
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i] <= out) out = v[i];
	}

	return out;
}

template<typename T>
void print_statistic(std::chrono::steady_clock::time_point end_m, std::chrono::steady_clock::time_point start_m, int turn, int pass, std::vector<T> v)
{
	std::cout << "\n______________________________________\n"
		<< "All time: " << time_calculate(end_m, start_m) << "\n"
		<< "Numder of iterations in 1 turn: " << turn << "\n"
		<< "Numder of pass: " << pass << "\n"
		<< "Number of all itr: " << ((int)turn * pass) << "\n"
		<< "Averange time for 1 turn: " << average(v) << "\n"
		<< "Max T: " << find_max(v) << "\n"
		<< "Min T: " << find_min(v) << "\n"
		<< "CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << "\n\n";
}


void calculate_energy(std::vector<double>& energy_vector, std::vector<double> time_vector, int proc)
{
	//0 - CPU, 1 - GPU, 3 - FPGA
	double average_wattage[] = { 13.5, 27., 1.5 };


	for (size_t i = 0; i < time_vector.size(); i++)
	{
		energy_vector.push_back(((time_vector[i] / (1000 * 60 * 60))) * average_wattage[proc]);
	}
}

void calculate_memory(DWORDLONG& totalVirtualMem, DWORDLONG& totalPhysMem, SIZE_T& physMemUsedByCurProc)
{
	MEMORYSTATUSEX memoryInfo;
	memoryInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memoryInfo);
	totalVirtualMem = memoryInfo.ullTotalPageFile;
	totalPhysMem = memoryInfo.ullTotalPhys;
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	physMemUsedByCurProc = pmc.WorkingSetSize;
}