#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>
#include <omp.h>

#include "Complex.h"
#include "glut.h"
//#include <iostream>

int WH = 500, WW = 500;
int nX = 500, nY = 500;
float X = 0, Y = 0, scale_factor = 1;

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
	
	switch (key) {
	case GLUT_KEY_LEFT:
		X = X - 2; break;
	case GLUT_KEY_UP:
		Y = Y + 2; break;
	case GLUT_KEY_RIGHT:
		X = X + 2; break;
	case GLUT_KEY_DOWN:
		Y = Y - 2; break;
	case GLUT_KEY_PAGE_UP:
		if (scale_factor > 10)
		{
			scale_factor = 1; break;
		}
		else
		{
			scale_factor = scale_factor + 0.1; break; break;
		}
	case GLUT_KEY_PAGE_DOWN:
		if (scale_factor < 1)
		{
			scale_factor = 1; break;
		}
		else
		{
			scale_factor = scale_factor - 0.1; break;
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
	
	glBegin(GL_POINTS);
	
	for (double x = -10; x < 10; x += 0.01)
	{
		glColor3f(1., 1., 1.);
		y = sin(x);
		glVertex2d(x*20, y*100);
		glColor3f(1., 1., 0.);
		glVertex2d(x * 20 + 20, y * 100);
		glColor3f(1., 0., 1.);
		glVertex2d(x * 20 + 40, y * 100);
		
		//std::this_thread::sleep_for(std::chrono::milliseconds(100);
	}
	
	
	glEnd();

	glTranslatef(X, Y, 0);
	glScalef(scale_factor, scale_factor, 0);

	glutSwapBuffers();
}

void displayM()
{
	double r, g, b;
	int xx = 0, yy = 0;
	nX = 500; nY = 500;
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);

#pragma omp for firstprivate(x) lastprivate(y)
		for (int y = (nY - 500); y < nY; y++) //построение множества
		{
			xx = 0;
			for (int x = (nX - 500); x < nX; x++)
			{
				Complex z(0, 0);
				int i = 0;
				
				while (i < 500 && z.abs() < 16)
				{
					z = z * z + Complex((x - nX / 1.5) / 180.0, (y - nY / 2.) / 180.0);
					i++;
				}
				
				r = 0.7 + i * 0.05 * 0.7;
				g = 0.2 + i * 0.03 * 0.2;
				b = 0.2 + i * 0.03 * 0.2;
				
				glColor3d(r, g, b);
				glVertex2d(xx, yy);

				xx++;
			}
			yy++;
		}

	glEnd();
	
	glutSwapBuffers();
}

int main( int argc, char **argv )
{
	/*glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WW, WH);
	glutCreateWindow("test");
	glutDisplayFunc(displayT);
	glutReshapeFunc(reshape);
	glutIdleFunc(displayT);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF); 
	glutIgnoreKeyRepeat(1);
	glutSpecialFunc(fProp);
	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(fPropR);*/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WW, WH);
	glutCreateWindow("Mandelbrot");
	glutDisplayFunc(displayM);
	glutReshapeFunc(reshape2);
	glutIdleFunc(displayM);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutIgnoreKeyRepeat(1);
	glutSpecialFunc(fProp);
	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(fPropR);

	glutMainLoop();
	return 0;
}