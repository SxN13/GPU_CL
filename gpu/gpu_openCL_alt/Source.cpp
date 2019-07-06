#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#include <chrono>
//#ifdef __APPLE__
//#include <OpenCL/opencl.h>
//#else
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
//#endif

#include "Prototype_test.h"

#define MAX_SOURCE_SIZE (0x1000000)

int main(void) 
{
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
	const int LIST_SIZE = 1000;

	int *A = (int*)malloc(sizeof(int)*LIST_SIZE);
	int *B = (int*)malloc(sizeof(int)*LIST_SIZE);
	
	for (i = 0; i < LIST_SIZE; i++) 
	{
		A[i] = i + rand() % 10;
		B[i] = i + rand() % 10;
	}

	// ��� ������� � source_str
	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("Source.cl", "r");
	if (!fp) 
	{
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	// ��������� id ��������
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	// �������� ��������� OpenCL
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	// �������� ������� ������
	//cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
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
	cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);

	// �������� ������� � �������� ���������
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

	// Execute the OpenCL kernel on the list
	size_t global_item_size = LIST_SIZE; // Process the entire lists
	size_t local_item_size = 512; // Divide work items into groups of 64

	int *C;

	start_m = std::chrono::steady_clock::now();
	for (int i = 0; i < 1000; i++)
	{
		std::cout << i << "\n";
		start = std::chrono::steady_clock::now();
		
		ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
		// Read the memory buffer C on the device to the local variable C
		C = (int*)malloc(sizeof(int)*LIST_SIZE);
		ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), C, 0, NULL, NULL);
		
		end = std::chrono::steady_clock::now();
		// ����� � �������������
		seconds = time_calculate(start, end);
		time_vector.push_back(seconds);
	}
	end_m = std::chrono::steady_clock::now();

	// ����������� �����������
	//for (i = 0; i < LIST_SIZE; i++)
		//printf("%d + %d = %d\n", A[i], B[i], C[i]);
	print_statistic(end_m, start_m, 1000, 1000, time_vector);

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
	
	//������
	free(A);
	free(B);
	free(C);

	system("pause");
	return 0;
}