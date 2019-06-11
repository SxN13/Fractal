/*#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>
#include <omp.h>

#include "Complex.h"
#include "glut.h"

int WH = 500, WW = 500;
int nX = 500, nY = 500;
float X = 0, Y = 0, scale_factor = 1.0f, iscf = scale_factor;
int iX = X, iY = Y;

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-w, w, -h, h);

	glTranslatef(X, Y, 0);
	glScalef(scale_factor, scale_factor, scale_factor);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

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
		X = X + 2; break;
	case GLUT_KEY_UP:
		Y = Y - 2; break;
	case GLUT_KEY_RIGHT:
		X = X - 2; break;
	case GLUT_KEY_DOWN:
		Y = Y + 2; break;
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
		scale_factor = 1; break;
	case GLUT_KEY_PAGE_DOWN:
		scale_factor = 1; break;
	}
}

void displayT()
{
	double y = 0.;
	
	glClear( GL_COLOR_BUFFER_BIT );
	glBegin(GL_LINES);

	glColor3f(1., 0, 0);
	glVertex2d(-WW, 0.);
	glVertex2d(WW, 0.);
	glVertex2d(0., -WH);
	glVertex2d(0., WH);
	
	glEnd();
	
	int i = 0;

	while (i < 10) {

		glBegin(GL_POINTS);

		for (double x = -10; x < 10; x += 0.001)
		{

			glColor3f(1., 1., 1.);
			y = cos(exp(x)) + sin(x);
			glVertex2d(x * 20, y * 100);
			glColor3f(1., 1., 0.);
			glVertex2d(x * 20 + 20, y * 100);
			glColor3f(1., 0., 1.);
			glVertex2d(x * 20 + 40, y * 100);
			glScalef(scale_factor + 0.01, scale_factor + 0.01, 0);
			
		}

		glEnd();
		i++;
	}

	glTranslatef(X, Y, 0);
	glScalef(scale_factor, scale_factor, 0);

	glutSwapBuffers();

}

void displayM()
{
	double r, g, b;
	int xx = 0, yy = 0;
	nX = WW; nY = WH; //Менять с размером окна!!!!!!
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	//0.0

	double offsetR = 1.5, offsetI = 2.; //Адекват 1.5 и 2.
	double сглаживание = 8; //сглаживание
		for (int y = 0; y < nY; y++) //построение множества
		{
			xx = 0;
			for (int x = 0; x < nX; x++)
			{
				Complex z(0, 0);
				int i = 0;
				
				//краски
				while (i < 100 && z.abs() < сглаживание)
				{
					z = z * z + Complex((x - nX / offsetR) / 180.0, (y - nY / offsetI) / 180.0);
					i++;
				}
				
				r = 0. + i * 0.05 * 0.7;
				g = 0. + i * 0.05 * 0.1;
				b = 0. + i * 0.05 * 0.5;
				
				//Краски закончились
				glColor3d(r, g, b);
				glVertex2d(xx, yy);

				xx++;
			}
			yy++;
		}

	glEnd();
	
	glTranslatef(X, Y, 0);
	glScalef(scale_factor, scale_factor, 0);

	glutSwapBuffers();
}


int main( int argc, char **argv )
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
}*/