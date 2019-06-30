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
	
	int ret = 0;
	cl_platform_id platform_id = 0;
	cl_uint ret_num_platforms = 0, ret_num_devices = 0;
	cl_device_id device_id = 0;
	//cl_context context;
	//cl_command_queue command_queue;

	cl_uint numPlatformsToCheck = 6;
	cl_uint numPlatforms;
	clGetPlatformIDs(numPlatformsToCheck, NULL, &numPlatforms);
	cl_platform_id * platforms = new cl_platform_id[numPlatforms];

	cl_queue_properties props[] = { CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0 };
	
	
	/* получить доступные платформы */
	ret = clGetPlatformIDs(1, /*&platform_id*/ platforms, /*&ret_num_platforms*/ &numPlatforms);

	/* получить доступные устройства CL_DEVICE_TYPE_GPU/CL_DEVICE_TYPE_CPU */
	ret = clGetDeviceIDs( /*&platform_id*/ *platforms, CL_DEVICE_TYPE_GPU, 1, &device_id,/*&ret_num_platforms*/ &numPlatforms);

	/* создать контекст */
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	/* создаем команду */
	cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret); 
									/*clCreateCommandQueueWithProperties(context, device_id, props, &ret);*/

	cl_program program = NULL;
	cl_kernel kernel = NULL;

	FILE *fp;
	const char fileName[] = "Source.cl";
	
	char *source_str;
	size_t source_size;
	std::unique_ptr<char[]> source;

	try {
		std::ifstream stream;
		stream.exceptions(std::ios_base::badbit | std::ios_base::failbit | std::ios_base::eofbit);
		stream.open(fileName, std::ios_base::in);
		stream.seekg(0, std::ios_base::end);
		size_t source_size = stream.tellg();
		stream.seekg(0, std::ios_base::beg);
		source.reset(new char[source_size]);
		stream.read(source.get(), source_size);
	}
	catch (const std::exception& e) {
		std::cerr << "Can't read kernel: " << e.what() << std::endl;
		exit(1);
	}

	/* создать бинарник из кода программы */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

	/* скомпилировать программу */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	/* создать кернел */
	kernel = clCreateKernel(program, "test", &ret);

	cl_mem memobj = NULL;
	int memLenth = 10;
	cl_int* mem = (cl_int *)malloc(sizeof(cl_int)*memLenth);

	/* создать буфер */
	memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, memLenth * sizeof(cl_int), NULL, &ret);

	/* записать данные в буфер */ //Что-то здесь не так!!!
	ret = clEnqueueWriteBuffer(command_queue, memobj, CL_TRUE, 0, memLenth * sizeof(cl_int), mem, 0, NULL, NULL);

	/* устанавливаем параметр */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);

	size_t global_work_size[1] = { 10 };

	/* выполнить кернел */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL);

	/* считать данные из буфера */
	ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, memLenth * sizeof(float), mem, 0, NULL, NULL);

	std::cout << "end";

	getchar();

	system("pause");
}


int main() {

	std::cout << sizeof(cl_int);

	init();

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

	//Входные вектора
	std::vector<int> a(0), b(0);
	//Инициализация входных векторов
	a = init_vector(1000, 200);
	b = init_vector(1000, 200);

	//Резервная полуавтоматическая инициализация входных векторов
//int *size = new int(1000);
//for (int i = 0; i < *size; i++)
//{
//	a.push_back(i + rand() % 100);
//	b.push_back(i + rand() % 100);
//}
//delete size;

	//Главный цикл выполняюций заданное количество тестов
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


