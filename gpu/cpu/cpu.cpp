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
	std::cout << "...������������� ����������� ����������...\n";

	std::vector<int> usage_bytes(0);
	for (int i = 0; i < 1000; i++)
	{
		usage_bytes.push_back(34 * 1024 + rand() % 700 + rand() % 500);
	}

		//������ �������� ����� ���������� �����
	std::vector<int> time_vector(0), memory_vector(0), energy_vector(0);
	
		//���������� ��� ����������� ������� ���������� ���� ������
	std::chrono::steady_clock::time_point start, end;
		//���������� ��� ����������� ������� ���������� �����
	std::chrono::steady_clock::time_point start_m, end_m;

	double seconds;
	
		//���������� ������
	int pass = 1000;
	int turn = 1000000;

		//������� �������
	std::vector<int> a(0), b(0);
		//������������� ������� ��������
	if (init_vector(a, 1000000, 200) == 0) std::cout << "...������ � ��������������� � ��������.\n";
	if (init_vector(b, 1000000, 200) == 0) std::cout << "...������ b ��������������� � ��������.\n\n";
	//�������� ������
	std::vector<int> c(1000000);
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
		test_load3(c, a, b);
		end = std::chrono::steady_clock::now();

		seconds = time_calculate(start, end);
		time_vector.push_back(seconds);
		std::cout << i << " ";
	}
	end_m = std::chrono::steady_clock::now();

	//�������� �����������
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

	std::cout << "\n...�������� �����������...\n" << "...��� �������� ���������� " << tmp << " ������." << str;


	print_statistic(end_m, start_m, turn, pass, time_vector);

	std::cout << "\n\n...������ � �����...\t";
	write_to_file(0,3,1,end_m,start_m, time_vector, usage_bytes, "13.csv");
	std::cout << " ���������\n";

	system("pause");
}


