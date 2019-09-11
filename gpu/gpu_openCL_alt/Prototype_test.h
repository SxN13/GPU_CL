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

//////��������� �������//////

//������������� ��������� + ��� �������� ��������
template<typename T>
std::vector<T> operator+ (std::vector<T> a, std::vector<T> b);
template<typename T>
std::vector<T> operator- (std::vector<T> a, std::vector<T> b);
template<typename T>
std::vector<T> operator* (std::vector<T> a, std::vector<T> b);
//���������� ������� �������� �������� �������
template<typename T>
double average(std::vector<T> v);
//������ � ���� csv ����������� ����������� ������
template<typename T>
void write_to_file(clock_t end_m, clock_t start_m, int turn, int pass, std::vector<T> v, std::string processor, std::string test_pack, std::vector<int> bytes, std::string file_name);
//�������� �������� �1
void test_load(double turn);
//�������� �������� �2, ���������� ��������� �������� ������� �������� � � �
template<typename T>
std::vector<T> test_load1(int turn, std::vector<T> a, std::vector<T> b);
//�������� �������� �2, ���������� ��������� ��������� ������� �������� � � �
template<typename T>
std::vector<T> test_load2(int turn, std::vector<T> a, std::vector<T> b);
//�������� �������� �2, ���������� ��������� ��������� ������� �������� � � �
template<typename T>
std::vector<T> test_load3(int turn, std::vector<T> a, std::vector<T> b);
//���������� ���������� ������ � �������� size, �������� �������� - ���������� ����� + �������� seed
std::vector<int> init_vector(int size, int seed);
//���������� ������������ ������� � �������� �������
template<typename T>
double find_max(std::vector<T> v);
//���������� ����������� ������� � �������� �������
template<typename T>
double find_min(std::vector<T> v);
//����� �� ����� ���������� ������
template<typename T>
void print_statistic(clock_t end_m, clock_t start_m, int turn, int pass, std::vector<T> v);
//������ ������� ����������
double time_calculate(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end);
//��������� ������ �� ������ ����������
void getMemInfo(DWORDLONG &totalVirtualMem, DWORDLONG &totalPhysMem, SIZE_T &physMemUsedByCurProc);
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

template<typename T>
bool write_to_file(std::chrono::steady_clock::time_point end_m,
	std::chrono::steady_clock::time_point start_m,
	int turn,
	int pass,
	std::vector<T> v,
	std::string processor,
	std::string test_pack,
	std::vector<int> bytes,
	std::string file_name)
{
	std::ofstream file_out;
	file_out.open(file_name);

	file_out << "test_pack," << "processor," << "#iter," << "time," << "memory," << "turn," << "pass," << "all_iter," << "all_time," << "clock_per_sec" << "\n";
	std::string tmp = "";
	for (int i = 0; i < v.size(); i++)
	{
		file_out << test_pack << ","
			<< processor << ","
			<< i << ","
			<< v[i] << ","
			<< bytes[i] << ","
			<< turn << ","
			<< pass << ","
			<< turn * pass << ","
			<< time_calculate(end_m, start_m) << ","
			<< CLOCKS_PER_SEC << "\n";
	}
	file_out.close();
	return true;
}

void test_load(double turn)
{
	for (auto i = 0.; i < turn; i += 0.1)
	{
		if (i <= (i + 0.1)) {

		}
		else
		{

		}
	}
}

template<typename T>
std::vector<T> test_load1(int turn, std::vector<T> a, std::vector<T> b)
{
	// turn -> ���������� �������� ����� �������� ��������
	//� � � - ������� ������� ��� ��������
	std::vector<T> c;
	for (int i = 0; i < turn; i++) {
		c = a + b;
	}
	return c;
}

template<typename T>
std::vector<T> test_load2(int turn, std::vector<T> a, std::vector<T> b)
{
	// turn -> ���������� �������� ����� �������� ��������
	//� � � - ������� ������� ��� ���������
	std::vector<T> c;
	for (int i = 0; i < turn; i++) {
		c = a - b;
	}
	return c;
}

template<typename T>
std::vector<T> test_load3(int turn, std::vector<T> a, std::vector<T> b)
{
	// turn -> ���������� �������� ����� �������� ��������
	//� � � - ������� ������� ��� ���������
	std::vector<T> c;
	for (int i = 0; i < turn; i++) {
		c = a * b;
	}
	return c;
}

std::vector<int> init_vector(int size, int seed)
{
	//size - ������ �������
	//seed - �������� �������
	srand(seed);
	std::vector<int> out_vector(0);

	for (int i = 0; i < size; i++)
	{
		out_vector.push_back(i + rand() % 10);
	}

	return out_vector;
}

template<typename T>
void find_max_min(std::vector<T> v, std::vector<double>& max_min)
{
	max_min.push_back(0.);
	max_min.push_back(1000000.);
	for (int i = 0; i < v.size(); i++)
	{
		if (max_min[0] <= v[i]) max_min[0] = v[i];
		if (v[i] <= max_min[1]) max_min[1] = v[i];
	}
}

template<typename T>
void print_statistic(std::chrono::steady_clock::time_point end_m, std::chrono::steady_clock::time_point start_m, int turn, int pass, std::vector<T> v)
{
	std::vector<double> max_min(0);
	find_max_min(v, max_min);
	std::cout << "\n______________________________________\n"
		<< "All time: " << time_calculate(end_m, start_m) << "\n"
		<< "Numder of iterations in 1 turn: " << turn << "\n"
		<< "Numder of pass: " << pass << "\n"
		<< "Number of all itr: " << ((int)turn * pass) << "\n"
		<< "Averange time for 1 turn: " << average(v) << "\n"
		<< "Max T: " << max_min[0] << "\n"
		<< "Min T: " << max_min[1] << "\n"
		<< "CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << "\n\n";
}

void getMemInfo(DWORDLONG &totalVirtualMem, DWORDLONG &totalPhysMem, SIZE_T &physMemUsedByCurProc)
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

void calculate_energy(std::vector<double>& energy_vector, std::vector<double> time_vector, int proc)
{
	//������ ���������
	//0 - CPU, 1 - GPU, 3 - FPGA
	double average_wattage[] = { 13.5, 27., 1.5 };


	for (size_t i = 0; i < time_vector.size(); i++)
	{
		energy_vector.push_back(((time_vector[i] / (1000 * 60 * 60))) * average_wattage[proc]);
	}
}