//#include <CL/opencl.h>
#include <stdio.h> 
#include <ctime> 
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#include "Prototype_test.h"


int main() {
	
	std::vector<int> usage_bytes(0);
	for (int i = 0; i < 1000; i++)
	{
		usage_bytes.push_back(90 * 1024 + rand() % 700 + rand() % 500);
	}

		//������ �������� ����� ���������� �����
	std::vector<double> time_vector(0);
	
		//���������� ��� ����������� ������� ���������� ���� �����������
	std::chrono::steady_clock::time_point start, end;
		//���������� ��� ����������� ������� ���������� �����
	std::chrono::steady_clock::time_point start_m, end_m;

	double seconds;
	
		//���������� ������
	int pass = 1000;
	
		//���������� �������� � �����
	int turn = 1000;

		//������� �������
	std::vector<int> a(0), b(0);
		//������������� ������� ��������
	a = init_vector(1000, 200);
	b = init_vector(1000, 200);

		//��������� ������������������ ������������� ������� ��������
	//int *size = new int(1000);
	//for (int i = 0; i < *size; i++)
	//{
	//	a.push_back(i + rand() % 100);
	//	b.push_back(i + rand() % 100);
	//}
	//delete size;

		//������� ���� ����������� �������� ���������� ������
	start_m = std::chrono::steady_clock::now();
	for (int i = 0; i < pass; i++)
	{
		start = std::chrono::steady_clock::now();
		//test_load1(turn, a, b);
		//test_load2(turn, a, b);
		test_load3(turn, a, b);
		end = std::chrono::steady_clock::now();

		seconds = time_calculate(start, end);
		time_vector.push_back(seconds);
		std::cout << i << "\n";
	}
	end_m = std::chrono::steady_clock::now();

	print_statistic(end_m, start_m, turn, pass, time_vector);

	write_to_file(end_m, start_m, turn, pass, time_vector, "CPU", "CPU_PC", usage_bytes, "zero_test_pack*.csv");

	system("pause");
}


