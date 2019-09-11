/*
	Версия совмещающая в себе два разных алгоритма
*/

#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>
#include <omp.h>
#include <ctime>

#include "Complex.h"
#include "../FractalGlut/glut.h"

#include <windows.h>
#include <Psapi.h>
#pragma comment( lib, "psapi.lib" )

//#ifdef __APPLE__
//#include <OpenCL/opencl.h>
//#else
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
//#endif

#define MAX_SOURCE_SIZE (0x1000000)



int WH = 500, WW = 500;
int nX = 500, nY = 500;
float X = 0, Y = 0, scale_factor = 1.0f, iscf = scale_factor;
int iX = X, iY = Y;

double cent_X = -1.88488933694469, cent_y = 0.00000000081387;
//prototype
void reshape2(int w, int h);
void fProp(int key, int x, int y);
void displayM();
////////////


void reshape2(int w, int h)
{

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glTranslatef(X, Y, 0);
	glScalef(scale_factor, scale_factor, scale_factor);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}
void fProp(int key, int x, int y) {

	//#define GLUT_KEY_LEFT			100
	//#define GLUT_KEY_UP			101
	//#define GLUT_KEY_RIGHT		102
	//#define GLUT_KEY_DOWN			103
	//#define GLUT_KEY_PAGE_UP		104
	//#define GLUT_KEY_PAGE_DOWN	105
	//#define GLUT_KEY_HOME			106

	switch (key) {
	case GLUT_KEY_F9:
		scale_factor = iscf;
		X = Y = iX;
		break;
	case GLUT_KEY_LEFT:
		X = X + 10; break;
	case GLUT_KEY_UP:
		Y = Y - 10; break;
	case GLUT_KEY_RIGHT:
		X = X - 10; break;
	case GLUT_KEY_DOWN:
		Y = Y + 10; break;
	case GLUT_KEY_PAGE_UP:
		if (scale_factor > 100)
		{
			scale_factor = 1; break;
		}
		else
		{
			scale_factor = scale_factor + 0.05; break;
		}
	case GLUT_KEY_PAGE_DOWN:
		if (scale_factor < 1)
		{
			scale_factor = 1; break;
		}
		else
		{
			scale_factor = scale_factor - 0.05; break;
		}
	}
}
void fPropR(int key, int x, int y) {

	//#define GLUT_KEY_LEFT			100
	//#define GLUT_KEY_UP			101
	//#define GLUT_KEY_RIGHT		102
	//#define GLUT_KEY_DOWN			103
	//#define GLUT_KEY_PAGE_UP		104
	//#define GLUT_KEY_PAGE_DOWN	105

	switch (key) {
	case GLUT_KEY_F9:
		scale_factor = 1.f;
		X = Y = 0;
		break;
	case GLUT_KEY_LEFT:
		X = 0; break;
	case GLUT_KEY_UP:
		Y = 0; break;
	case GLUT_KEY_RIGHT:
		X = 0; break;
	case GLUT_KEY_DOWN:
		Y = 0; break;
	case GLUT_KEY_PAGE_UP:
		scale_factor = scale_factor; break;
	case GLUT_KEY_PAGE_DOWN:
		scale_factor = scale_factor; break;
	}
}


int calculateMandelbrot(double cr, double ci, double max_iterations) {
	int i = 0;
	double zr = 0.0, zi = 0.0;
	while (i < max_iterations && zr * zr + zi * zi < 4.0)
	{
		double temp = zr * zr - zi * zi + cr;
		zi = 2.0 * zr * zi + ci;
		zr = temp;
		i++;
	}
	return i;

}

double mapToReal(int x, int imageWidth, double minR, double maxR)
{
	double range = maxR - minR;
	return x * (range / imageWidth) + minR;

}
double mapToImaginary(int y, int imageHeight, double minI, double maxI)
{
	double range = maxI - minI;
	return y * (range / imageHeight) + minI;

}

void displayM()
{
	setlocale(0, "RUS");
	// Входные вектора
	int i;
	const int LIST_SIZE = 1;

	std::cout << "Memory allocation...\t";
	double* CI = (double*)malloc(sizeof(double) * LIST_SIZE);
	double* CR = (double*)malloc(sizeof(double) * LIST_SIZE);
	std::cout << "Done\n";


	// Код кернела в source_str
	FILE* fp;
	char* source_str;
	size_t source_size;

	std::cout << "Read kernel program...\t";
	fp = fopen("fractal.cl", "r");
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

	// Получение id платформ
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;

	std::cout << "Checking device...\tDone\n";
	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

	std::cout << "Get device id's...\tDone\n";
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	// Создания контекста OpenCL

	std::cout << "Create context...\tDone\n";
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	// Создание очередь команд
	//cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	std::cout << "Create command queue...\tDone\n";
	cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret);

	// Создание буферов для векторов 
	cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);
	cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);
	cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);

	// Перенос векторов в буферы для отправки на GPU
	ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), CI, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), CR, 0, NULL, NULL);

	// Программа кернела
	cl_program program = clCreateProgramWithSource(context, 1, (const char**)& source_str, (const size_t*)& source_size, &ret);

	// Компиляция программы
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	// Создание кернела OpenCL
	cl_kernel kernel = clCreateKernel(program, "calc", &ret);

	// Датасеты
	// GPU write_to_file(end_m, start_m, turn, pass, time_vector, "GPU", "GPU_CL", usage_bytes, "gpu_test_pack_mul.csv")
	// CPU write_to_file(end_m, start_m, turn, pass, cpu_time, "CPU", "CPU_CL", usage_bytes, "cpu_test_pack.csv")

	/*
	if (write_to_file(end_m, start_m, turn, pass, time_vector, "CPU", "CPU_CL", usage_bytes, "cpu_test_pack_mul.csv"))
	std::cout << "Export file... \t\t\tDone\n";
	*/

	//////////////////////////////////////////////////////////////////////////
	nX = WW; nY = WH; //Менять с размером окна!!!!!!
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	////////////////////////////////////////////////////////////////////////0.0

	int imageWidth, imageHeight, maxN;
	double minR, maxR, minI, maxI;

	imageWidth = WW;
	imageHeight = WH;
	maxN = 256;

	//вынести переменные и итерировать их при приближении

	minR = -1.5;
	maxR = 0.7;
	minI = -1.0;
	maxI = 1.0;

	// Выделение места под пул глобального массива данных
	size_t global_item_size = LIST_SIZE; // Длинна входного списка
	/*
		8 разрядов для CPU
		64 разряда для GPU
	*/
	size_t local_item_size = 64; // Деление входных данных на группы для устройства

	//Выходной массив 
	int* C;

	// Выделение места под С
	C = (int*)malloc(sizeof(int) * LIST_SIZE);

	for (int y = 0; y < imageHeight; y++) //строки
	{
		for (int x = 0; x < imageWidth; x++) //пиксели в строке
		{

			CR[0] = mapToReal(x, imageWidth * (scale_factor), minR, maxR);
			CI[0] = mapToImaginary(y, imageHeight * (scale_factor), minI, maxI);
			// Передача буферов в качестве аргумента
			ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)& a_mem_obj);
			ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)& b_mem_obj);
			ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)& c_mem_obj);

			// Выполнение кернела на девайсе
			ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
			// Считывание выходного буфера и копирование его в массив С

			ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), C, 0, NULL, NULL);

			int r = 0. + C[0] * 0.05 * 0.7;
			int g = 0. + C[0] * 0.05 * 0.1;
			int b = 0. + C[0] * 0.05 * 0.5;

			glColor3d(r, g, b);
			glVertex2d(x, y);
		}
	}

	///////////////////////////////////////////////////////////////////
	glEnd();

	glTranslatef(X, Y, 0);
	//glScalef(scale_factor, scale_factor, 0);
	glutSwapBuffers();


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

	// Особождение выделяемой памяти
	free(CI);
	free(CR);
	free(C);

}


int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WW, WH);
	glutCreateWindow("Mandelbrot");

	glutDisplayFunc(displayM);
	glutReshapeFunc(reshape2); //Решейп
	glutIdleFunc(displayM);

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutIgnoreKeyRepeat(1);
	glutSpecialFunc(fProp);
	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(fPropR);

	glutMainLoop();

	return 0;
}

