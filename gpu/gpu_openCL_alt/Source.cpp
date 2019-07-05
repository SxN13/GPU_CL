#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>

#include "Prototype_test.h"

//#ifdef __APPLE__
//#include <OpenCL/opencl.h>
//#else
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
//#endif

#define MAX_SOURCE_SIZE (0x1000000)

int main(void) 
{
	srand(200);
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

	// Create the two input vectors
	int i;
	const int LIST_SIZE = 1000;
	int *A = (int*)malloc(sizeof(int)*LIST_SIZE);
	int *B = (int*)malloc(sizeof(int)*LIST_SIZE);
	for (i = 0; i < LIST_SIZE; i++) 
	{
		A[i] = i + rand() % 10;
		B[i] = i + rand() % 10;
	}

	// Load the kernel source code into the array source_str
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

	// Get platform and device information
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	// Create an OpenCL context
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	// Create a command queue
	//cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret);

	// Create memory buffers on the device for each vector 
	cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);
	cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);
	cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);

	// Copy the lists A and B to their respective memory buffers
	ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), A, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), B, 0, NULL, NULL);

	// Create a program from the kernel source
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

	// Build the program
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	// Create the OpenCL kernel
	cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);

	// Set the arguments of the kernel
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

	// Execute the OpenCL kernel on the list
	size_t global_item_size = LIST_SIZE; // Process the entire lists
	size_t local_item_size = 64; // Divide work items into groups of 64
	
	start_m = clock();
	for (int i = 0; i < 1000; i++)
	{
		std::cout << i << "\n";
		start = clock();
		ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
		end = clock();
		seconds = time_calculate(start, end);
		time_vector.push_back(seconds);
	}
	end_m = clock();
	// Read the memory buffer C on the device to the local variable C
	int *C = (int*)malloc(sizeof(int)*LIST_SIZE);
	ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), C, 0, NULL, NULL);

	// Отображение результатов
	//for (i = 0; i < LIST_SIZE; i++)
		//printf("%d + %d = %d\n", A[i], B[i], C[i]);
	print_statistic(end_m, start_m, 1000, 1000, time_vector);

	for (int i = 0; i < time_vector.size(); i++)
	{
		std::cout << time_vector[i] << "\n";
	}

	// Очистка памяти
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(a_mem_obj);
	ret = clReleaseMemObject(b_mem_obj);
	ret = clReleaseMemObject(c_mem_obj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	free(A);
	free(B);
	free(C);

	system("pause");
	return 0;
}