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
#include "glut.h"

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


int findMandelbrot(double cr, double ci, double max_iterations) {
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

	int iter = imageWidth * imageHeight;
	int counterIter = 0;
	float pr = 0;

	for (int y = 0; y < imageHeight; y++) //строки
	{
		for (int x = 0; x < imageWidth; x++) //пиксели в строке
		{
		
			double cr = mapToReal(x, imageWidth * (scale_factor), minR, maxR);
			double ci = mapToImaginary(y, imageHeight * (scale_factor), minI, maxI);

			int n = findMandelbrot(cr, ci, maxN);

			int r = 0. + n * 0.05 * 0.7;
			int g = 0. + n * 0.05 * 0.1;
			int b = 0. + n * 0.05 * 0.5;

			glColor3d(r, g, b);
			glVertex2d(x, y);
			
		}
	}
	
	///////////////////////////////////////////////////////////////////
	glEnd();

	glTranslatef(X, Y, 0);
	//glScalef(scale_factor, scale_factor, 0);

	glutSwapBuffers();
}


int main(int argc, char **argv)
{
	time_t t1; time_t t2;
	time(&t1);

	std::cout << "Hi!";
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

	time(&t2);
	std::cout << "\n" << difftime(t2, t1);

	glutMainLoop();

	return 0;
}