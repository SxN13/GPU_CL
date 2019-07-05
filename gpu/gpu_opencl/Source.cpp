#define _CRT_SECURE_NO_WARNINGS

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>

#include <stdio.h> 
#include <ctime> 
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#include "Prototype_test.h"



void init() {
	
	std::string clSrc =
		"__kernel void test(__global int* message[])"
		"{"
		"	int gid = get_global_id(0);"
		"	message[gid] += gid;"
		"}";

	int ret = 0;
	cl_platform_id platform_id = 0;
	cl_uint ret_num_platforms = 0, ret_num_devices = 0;
	cl_device_id device_id = 0;
	//cl_context context;
	//cl_command_queue command_queue;

	cl_uint numPlatformsToCheck = 6;
	cl_uint numPlatforms;
	ret = clGetPlatformIDs(numPlatformsToCheck, NULL, &numPlatforms);
	cl_platform_id * platforms = new cl_platform_id[numPlatforms];

	cl_queue_properties props[] = { CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0 };
	
	
	/* �������� ��������� ��������� */
	ret = clGetPlatformIDs(1, /*&platform_id*/ platforms, /*&ret_num_platforms*/ &numPlatforms);

	/* �������� ��������� ���������� CL_DEVICE_TYPE_GPU/CL_DEVICE_TYPE_CPU */
	ret = clGetDeviceIDs( /*&platform_id*/ *platforms, CL_DEVICE_TYPE_GPU, 1, &device_id,/*&ret_num_platforms*/ &numPlatforms);

	/* ������� �������� */
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	/* ������� ������� */
	cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, NULL, &ret); 
									/*clCreateCommandQueueWithProperties(context, device_id, props, &ret);*/

	cl_program program = NULL;
	cl_kernel kernel = NULL;

	FILE *fp;
	const char fileName[] = "Source.cl";
	
	char *source_str;
	size_t source_size;
	std::unique_ptr<char[]> source;

	std::ifstream sourceFile("Source.cl");
	std::string sourceCode(std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));

	/* ������� �������� �� ���� ��������� */
	source_size = 1024;
	/*�������� ���� ��������� �� �����*/
	program = clCreateProgramWithSource(context, 1, (const char **)&sourceCode, (const size_t *)&source_size, &ret);
	/*�������� ����� ��������� �� ���������� std::string*/
	//program = clCreateProgramWithSource(context, 1, (const char **)&clSrc, (const size_t *)&source_size, &ret);
		
	/* �������������� ��������� */
	//ret = clBuildProgram(program, 0, &device_id, NULL, NULL, NULL);
	//ret = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	
	/* ������� ������ */
	kernel = clCreateKernel(program, "test", &ret);

	cl_mem memobj = NULL;
	int memLenth = 10;
	cl_int* mem = (cl_int *)malloc(sizeof(cl_int)*memLenth);

	/* ������� ����� */
	memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, memLenth * sizeof(cl_int), NULL, &ret);

	/* �������� ������ � ����� */ //���-�� ����� �� ���!!!
	ret = clEnqueueWriteBuffer(command_queue, memobj, CL_TRUE, 0, memLenth * sizeof(cl_int), mem, 0, NULL, NULL);

	/* ������������� �������� */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);

	size_t global_work_size[1] = { 10 };

	/* ��������� ������ */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL);

	/* ������� ������ �� ������ */
	ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, memLenth * sizeof(float), mem, 0, NULL, NULL);

	std::cout << "\nend\n" << mem << "\n" << memobj;

	//getchar();

	//system("pause");

}


int main() {

	std::cout << sizeof(cl_int);

	init();

	//������ �������� ����� ���������� �����
	std::vector<double> time_vector(0);

	//���������� ��� ����������� ������� ���������� ���� �����������
	clock_t start_m, end_m;
	//���������� ��� ����������� ������� ���������� �����
	clock_t start, end;

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
	//write_to_file(end_m, start_m, turn, pass, time_vector, "CPU", "1PC", "1kx1k_vector_mul.csv");

	system("pause");
}


