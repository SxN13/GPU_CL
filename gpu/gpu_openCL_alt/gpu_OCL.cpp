#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#include <chrono>

#include <windows.h>
#include <Psapi.h>
#pragma comment( lib, "psapi.lib" )

//#ifdef __APPLE__
//#include <OpenCL/opencl.h>
//#else
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
//#endif

#include "../cpu/Prototype_test.h"

#define MAX_SOURCE_SIZE (0x1000000)

int main(void) 
{
	setlocale(0, "RUS");
	std::vector<int> usage_bytes(0);
	for (int i = 0; i < 1000; i++)
	{
		usage_bytes.push_back(97280 + rand() % 2048);
	}

	// ��� ��������� ���������� � ����������� ������ ���������� � ������
	DWORDLONG totalPhysMem, totalVirtMem;
	// ������������� ���������� ������ ���������� ��������� ����� � ������
	SIZE_T usedPhysMemByCurProc;

	calculate_memory(totalVirtMem, totalPhysMem, usedPhysMemByCurProc);

	std::cout << "Virtual memory -> \t\t\t\t" << totalVirtMem << " bytes\n";
	std::cout << "Physical memory -> \t\t\t\t" << totalPhysMem << " bytes\n";
	std::cout << "Used physical memory by current process -> \t" << usedPhysMemByCurProc << " bytes\n";

	srand(200);
	//������ �������� ����� ���������� �����
	std::vector<double> time_vector(0);
	//���������� ��� ����������� ������� ���������� ���� �����������
	std::chrono::steady_clock::time_point start_m, end_m;
	//���������� ��� ����������� ������� ���������� �����
	std::chrono::steady_clock::time_point start, end;
	double seconds;
	//���������� ������
	int pass = 1000;
	//���������� �������� � �����
	int turn = 1000;

	// ������� �������
	int i;
	const int LIST_SIZE = 1000000;

	std::cout << "Memory allocation...\t";
	int *A = (int*)malloc(sizeof(int)*LIST_SIZE);
	int *B = (int*)malloc(sizeof(int)*LIST_SIZE);
	std::cout << "Done\n";

	for (i = 0; i < LIST_SIZE; i++) 
	{
		A[i] = i + rand() % 10;
		B[i] = i + rand() % 10;
	}

	// ��� ������� � source_str
	FILE *fp;
	char *source_str;
	size_t source_size;

	std::cout << "Read kernel program...\t";
	fp = fopen("Source.cl", "r");
	if (!fp) 
	{
		std::cout << "Fail\n";
		fprintf(stderr, "KErnel file is lost\n");
		system("pause");
		exit(1);
	}
	std::cout << "Done\n";

	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	// ��������� id ��������
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;

	std::cout << "Checking device...\tDone\n";
	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

	std::cout << "Get device id's...\tDone\n";
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	// �������� ��������� OpenCL

	std::cout << "Create context...\tDone\n";
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	// �������� ������� ������
	//cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	std::cout << "Create command queue...\tDone\n";
	cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret);

	// �������� ������� ��� �������� 
	cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);
	cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);
	cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);

	// ������� �������� � ������ ��� �������� �� GPU
	ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), A, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), B, 0, NULL, NULL);

	// ��������� �������
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

	// ���������� ���������
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	// �������� ������� OpenCL
	cl_kernel kernel = clCreateKernel(program, "add", &ret);

	// �������� ������� � �������� ���������
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

	// ��������� ����� ��� ��� ����������� ������� ������
	size_t global_item_size = LIST_SIZE; // ������ �������� ������
	/*
		8 �������� ��� CPU
		64 ������� ��� GPU
	*/
	size_t local_item_size = 64; // ������� ������� ������ �� ������ ��� ����������

	//�������� ������ 
	int *C;

	// ��������� ����� ��� �
	C = (int*)malloc(sizeof(int)*LIST_SIZE); 

	// ������� �������� ������
	std::vector<size_t> physical_memory_total(0), virtual_memory_total(0), physical_memory_used(0);

	start_m = std::chrono::steady_clock::now();
	for (int i = 0; i < 1000; i++)
	{
		// ������ ���������� � ������ �����
		std::cout << i << " ";
		start = std::chrono::steady_clock::now();
		
		// ��������� ���������� �� ������
		calculate_memory(totalVirtMem, totalPhysMem, usedPhysMemByCurProc);
		physical_memory_total.push_back(totalPhysMem / 1024 / 1024);
		virtual_memory_total.push_back(totalVirtMem / 1024 / 1024);
		physical_memory_used.push_back(usedPhysMemByCurProc / 1024 / 1024);

		// ���������� ������� �� �������
		ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
		// ���������� ��������� ������ � ����������� ��� � ������ �
		ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), C, 0, NULL, NULL);
		
		end = std::chrono::steady_clock::now();
		// ����� � �������������
		seconds = time_calculate(start, end);
		time_vector.push_back(seconds);
	}
	end_m = std::chrono::steady_clock::now();

	// ����������� �����������
		//�������� �����������
	int err_count = 0;
	std::string str = "";
	for (size_t i = 0; i < LIST_SIZE; i++)
	{
		if (A[i] + B[i] != C[i])
		{
			err_count += 1;
			str += "\n";
			str += std::to_string(A[i]);
			str += " + ";
			str += std::to_string(B[i]);
			str += " = ";
			str += std::to_string(C[i]);
			str += "\n";
		}
	}

	std::cout << "\n...�������� �����������...\n" << "...��� �������� ���������� " << err_count << " ������." << str;
	
	// ����� ����������
	print_statistic(end_m, start_m, 1000, 1000, time_vector);

	/*for (int i = 0; i < physical_memory_total.size(); i++)
	{
		std::cout << physical_memory_total[i] << "\t" << physical_memory_used[i] << "\t" << virtual_memory_total[i] << "\n";
	}*/

	// ������� ������
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(a_mem_obj);
	ret = clReleaseMemObject(b_mem_obj);
	ret = clReleaseMemObject(c_mem_obj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	
	// ����������� ���������� ������
	free(A);
	free(B);
	free(C);

	// ��������
	// GPU write_to_file(end_m, start_m, turn, pass, time_vector, "GPU", "GPU_CL", usage_bytes, "gpu_test_pack_mul.csv")
	// CPU write_to_file(end_m, start_m, turn, pass, cpu_time, "CPU", "CPU_CL", usage_bytes, "cpu_test_pack.csv")

	/*if (write_to_file(end_m, start_m, turn, pass, time_vector, "CPU", "CPU_CL", usage_bytes, "cpu_test_pack_mul.csv"))
	std::cout << "Export file... \t\t\tDone\n";*/

	system("pause");
	return 0;
}